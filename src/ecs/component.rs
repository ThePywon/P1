use std::{any::{Any, TypeId}, collections::HashMap, fmt::Display, sync::{Arc, Mutex, RwLock}};
use bitvec::{order::Lsb0, view::BitView};

use super::ComponentError;

pub trait Component: Send + Sync {
  //const ID: u32;
}

// Singleton to store my registered components AND do component id shenanegans
pub(crate) struct ComponentIndexer {
  registered_components: Mutex<HashMap<TypeId, u32>>
}

impl ComponentIndexer {
  pub fn new<const B: usize>() -> Self {
    ComponentIndexer { registered_components: Mutex::new(HashMap::with_capacity(B*8)) }
  }

  pub fn register_components(&mut self, components: Vec<TypeId>) -> Result<(), ComponentError> {
    let mut map = self.registered_components.lock().unwrap();
    if map.len() + components.len() > map.capacity() {
      return Err(ComponentError::NotEnoughAllocatedSpace);
    }

    for component in components.iter() {
      if map.contains_key(component) { continue }

      let id = map.len() as u32;
      map.insert(*component, id);
    }

    Ok(())
  }

  pub fn get_component_id(&self, component: &TypeId) -> Result<u32, ComponentError> {
    self.registered_components.lock().unwrap().get(component)
      .copied().ok_or(ComponentError::Unregistered)
  }
}

// Maybe implement readonly bitmasks soon so this doesn't create a copy all the time
// Probably also convertions to and from unsigned types
#[derive(Clone, Copy, Hash, PartialEq, Eq)]
pub struct ComponentBitmask<const B: usize> {
  bitmask: [u8; B],
}

impl<const B: usize> ComponentBitmask<B> {
  pub(crate) fn new_empty() -> Self {
    ComponentBitmask { bitmask: [0u8; B] }
  }

  pub(crate) fn add(&mut self, index: u32) -> Result<(), ComponentError> {
    let mut bit = self.bitmask.view_bits_mut::<Lsb0>()
      .get_mut(index as usize).ok_or(ComponentError::InvalidId)?;
    *bit = true;

    Ok(())
  }

  #[allow(dead_code)]
  pub(crate) fn remove(&mut self, index: u32) -> Result<(), ComponentError> {
    let mut bit = self.bitmask.view_bits_mut::<Lsb0>()
      .get_mut(index as usize).ok_or(ComponentError::InvalidId)?;
    *bit = false;

    Ok(())
  }

  pub fn check_bit(&self, index: usize) -> Result<bool, ComponentError> {
    self.bitmask.view_bits::<Lsb0>().get(index)
      .map(|b| *b).ok_or(ComponentError::InvalidId)
  }

  pub fn contains(&self, other: ComponentBitmask<B>) -> bool {
    for (index, bit) in self.bitmask.view_bits::<Lsb0>().iter().enumerate() {
      if *bit && !other.check_bit(index).unwrap() { return false }
    }

    true
  }

  pub fn iter(&self) -> bitvec::slice::Iter<u8, Lsb0> {
    self.bitmask.view_bits::<Lsb0>().iter()
  }
}

impl<const B: usize> From<u32> for ComponentBitmask<B> {
  fn from(value: u32) -> Self {
    let mut result = Self::new_empty();
    result.add(value).unwrap();
    result
  }
}

impl<const B: usize> Display for ComponentBitmask<B> {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{}", self.iter().map(|b| if *b { "1" } else { "0" }).fold(String::new(), |a, b| a + b))
  }
}

pub(crate) type ComponentCell = Arc<RwLock<dyn Any + Send + Sync>>;
type ComponentRow = HashMap<u32, ComponentCell>;

pub(crate) struct ComponentManager<const B: usize> {
  components: HashMap<u32, ComponentRow>
}

impl<const B: usize> ComponentManager<B> {
  pub fn new() -> Self {
    ComponentManager { components: HashMap::new() }
  }

  // yeah need to change this shit
  // just end up accepting that invalid component errors will be externally handled by ComponentIndexer and mention that assumption

  // No entity error checking done here, notice the return type.
  pub fn get_component(&self, entity: u32, c_id: u32) -> Result<ComponentCell, ComponentError> {
    let b = self.components.get(&c_id).ok_or(ComponentError::InvalidId)?
            .get(&entity).ok_or(ComponentError::NotFoundForEntity)?;
    Ok(b.clone())
  }

  // No checks are done to ensure that the component id is correct
  // MUST be properly checked before passing to this method
  pub fn add_component<C: Component +  'static>(&mut self, entity: u32, c_id: u32, component: C) -> Result<(), ComponentError> {
    if let Some(pool) = self.components.get_mut(&c_id) {
      pool.insert(entity, Arc::new(RwLock::new(component)));
    } else {
      let mut pool: ComponentRow = HashMap::new();
      pool.insert(entity, Arc::new(RwLock::new(component)));
      self.components.insert(c_id, pool);
    }

    Ok(())
  }
}
