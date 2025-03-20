use std::{any::TypeId, collections::HashSet};
use dashmap::DashMap;

use super::Component;
use crate::error::DataError;

pub(crate) struct EntityManager {
  // Need this to get the actual components the entity has instead of the matching archetypes
  // Useful when archetypes get created or deleted or when the components change
  entities: DashMap<u32, Vec<TypeId>>,
  // Need this instead of self.entities.len() because later on will implement id recycling
  // So id allocation will become separate from self.entities.len()
  // Furthermore, this is not meant to be in a multithreaded context
  // This is because entity creation needs to happen in the main thread in order to update all archetypes properly from there
  next_entity_id: u32
}

impl EntityManager {
  pub fn new() -> Self {
    EntityManager { entities: DashMap::new(), next_entity_id: 0u32 }
  }

  pub fn create_entity(&mut self) -> u32 {
    let id = self.next_entity_id;
    self.next_entity_id += 1;
    self.entities.insert(id, Vec::new());
    id
  }

  pub fn has_component<C: Component>(&self, entity: u32) -> Result<bool, DataError> {
    Ok(self.entities.get(&entity).ok_or(DataError::EntityNotFound)?.contains(&TypeId::of::<C>()))
  }

  pub fn add_component<C: Component>(&mut self, entity: u32) -> Result<(), DataError> {
    let mut components = self.entities.get_mut(&entity).ok_or(DataError::EntityNotFound)?;
    let c_id = TypeId::of::<C>();
    if components.contains(&c_id) {
      return Err(DataError::ComponentExistsForEntity)
    } else {
      components.push(c_id);
    }

    Ok(())
  }

  pub fn get_archetype(&self, c_ids: &[TypeId]) -> Vec<u32> {
    let mut result = Vec::new();

    for entity in self.entities.iter() {
      let c_set: HashSet<_> = entity.value().iter().copied().collect();
      if c_ids.iter().all(|id| c_set.contains(id)) {
        result.push(*entity.key());
      }
    }

    result
  }
}
