use std::any::{Any, TypeId};
use std::hash::BuildHasherDefault;

use dashmap::Entry;
use dashmap::{
  mapref::one::{Ref, RefMut},
  DashMap,
};
use rustc_hash::FxHasher;

use crate::utility::ErasedMapContainer;

pub trait Component: Send + Sync + Any {
  // Need some input-controlled human-readable type names for component serialization
}

pub(crate) struct ComponentManager(
  DashMap<TypeId, ErasedMapContainer<u32>, BuildHasherDefault<FxHasher>>,
);

impl ComponentManager {
  pub fn new() -> Self {
    Self(DashMap::with_hasher(BuildHasherDefault::default()))
  }

  pub fn get_container<C: Component>(&self) -> Option<Ref<'_, TypeId, ErasedMapContainer<u32>>> {
    self.get_container_from_id(&TypeId::of::<C>())
  }
  pub fn get_container_from_id(
    &self,
    c_id: &TypeId,
  ) -> Option<Ref<'_, TypeId, ErasedMapContainer<u32>>> {
    self.0.get(c_id)
  }
  pub fn get_container_mut<C: Component>(
    &self,
  ) -> Option<RefMut<'_, TypeId, ErasedMapContainer<u32>>> {
    self.get_container_mut_from_id(&TypeId::of::<C>())
  }
  pub fn get_container_mut_from_id(
    &self,
    c_id: &TypeId,
  ) -> Option<RefMut<'_, TypeId, ErasedMapContainer<u32>>> {
    self.0.get_mut(c_id)
  }

  pub fn create_container<C: Component>(&mut self) -> RefMut<'_, TypeId, ErasedMapContainer<u32>> {
    match self.0.entry(TypeId::of::<C>()) {
      Entry::Vacant(entry) => entry.insert(ErasedMapContainer::new::<C>()),
      Entry::Occupied(entry) => entry.into_ref(),
    }
  }
}
