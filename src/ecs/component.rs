use std::any::{Any, TypeId};
use std::collections::{HashMap, hash_map::{Values, ValuesMut}};
use dashmap::Entry;
use dashmap::{DashMap, mapref::one::{Ref, RefMut}};

use super::{DataError, InternalDataError};

pub trait Component: Send + Sync + Any {
  // Need some input-controlled human-readable type names for component serialization
}

pub struct ComponentPtr(Box<dyn Any>);

impl ComponentPtr {
  pub fn new<C: Component>(component: C) -> Self {
    Self ( Box::new(component) )
  }

  pub fn cast_ref<C: Component>(&self) -> Option<&C> {
    self.0.downcast_ref::<C>()
  }
  pub fn cast_mut<C: Component>(&mut self) -> Option<&mut C> {
    self.0.downcast_mut::<C>()
  }
}

unsafe impl Send for ComponentPtr {}
unsafe impl Sync for ComponentPtr {}


pub struct ComponentContainer {
  component_type: TypeId,
  // Using HashMap here instead of DashMap because downcasting gets the inner value
  // Without regards to the lock, thus eliminating any potential thread safety we could've gotten there
  // To make this thread safe, I must ensure that the ComponentStorage is only ever accessed from a RwLock or the likes
  ptrs: HashMap<u32, ComponentPtr>
}

impl ComponentContainer {
  pub fn new<C: Component>() -> Self {
    Self { component_type: TypeId::of::<C>(), ptrs: HashMap::new() }
  }

  pub fn insert<C: Component>(&mut self, entity: u32, component: C) -> Result<(), DataError> {
    if TypeId::of::<C>() != self.component_type {
      return Err(DataError::Internal(InternalDataError::MismatchedComponentType))
    } else if self.ptrs.contains_key(&entity) {
      return Err(DataError::ComponentExistsForEntity)
    }

    self.ptrs.insert(entity, ComponentPtr::new(component));

    Ok(())
  }

  pub fn get<C: Component>(&self, entity: &u32) -> Option<&C> {
    self.ptrs.get(entity).and_then(|ptr| ptr.cast_ref::<C>())
  }

  pub fn get_mut<C: Component>(&mut self, entity: &u32) -> Option<&mut C> {
    self.ptrs.get_mut(entity).and_then(|ptr| ptr.cast_mut::<C>())
  }

  pub fn iter(&self) -> Values<'_, u32, ComponentPtr> {
    self.ptrs.values()
  }

  pub fn iter_mut(&mut self) -> ValuesMut<'_, u32, ComponentPtr> {
    self.ptrs.values_mut()
  }
}

unsafe impl Send for ComponentContainer {}
unsafe impl Sync for ComponentContainer {}

pub(crate) struct ComponentManager(DashMap<TypeId, ComponentContainer>);

impl ComponentManager {
  pub fn new() -> Self {
    Self ( DashMap::new() )
  }

  pub fn get_container<C: Component>(&self) -> Option<Ref<'_, TypeId, ComponentContainer>> {
    self.get_container_from_id(&TypeId::of::<C>())
  }
  pub fn get_container_from_id(&self, c_id: &TypeId) -> Option<Ref<'_, TypeId, ComponentContainer>> {
    self.0.get(c_id)
  }
  pub fn get_container_mut<C: Component>(&self) -> Option<RefMut<'_, TypeId, ComponentContainer>> {
    self.get_container_mut_from_id(&TypeId::of::<C>())
  }
  pub fn get_container_mut_from_id(&self, c_id: &TypeId) -> Option<RefMut<'_, TypeId, ComponentContainer>> {
    self.0.get_mut(c_id)
  }

  pub fn create_container<C: Component>(&mut self) -> RefMut<'_, TypeId, ComponentContainer> {
    match self.0.entry(TypeId::of::<C>()) {
      Entry::Vacant(entry) => entry.insert(ComponentContainer::new::<C>()),
      Entry::Occupied(entry) => entry.into_ref()
    }
  }
}
