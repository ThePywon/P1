use std::{any::TypeId, collections::HashMap, fmt::Display, sync::Mutex};
use bitvec::{order::Lsb0, view::BitView};

use super::ComponentError;

pub trait Component {
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

impl<const B: usize> Display for ComponentBitmask<B> {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{}", self.iter().map(|b| if *b { "1" } else { "0" }).fold(String::new(), |a, b| a + b))
  }
}
