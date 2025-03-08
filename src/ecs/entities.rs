use std::sync::Arc;
use parking_lot::RwLock;
use std::collections::HashMap;
use dashmap::DashMap;

use super::{ComponentBitmask, P1Error, EntityError};

// Note to self: Try not to import ComponentIndexer, operations here should not require it


type Entities = Arc<RwLock<Vec<u32>>>;

pub(crate) struct EntityManager<const B: usize> {
  // Need this to get the actual components the entity has instead of the matching archetypes
  // Useful when archetypes get created or deleted or when the components change
  entities: Arc<DashMap<u32, ComponentBitmask<B>>>,
  // Archetype entities are gonna match AT LEAST the component bitmask
  // Meaning that if negative matches are to be implemented later,
  // I will need 1 archetype for the positive query (could be ::new_empty())
  // and 1 archetype for the negative query and then compare both
  // Which will mean -neg queries > +pos queries in terms of frametime
  //
  // The lock is only for the Vec<u32> and not the entire IndexMap
  // Which technically means there is a risk of UB if the IndexMap has an insert or remove
  // In this case, I ensure that no accesses are done to inserted data before it is initialized
  // And I never do deletions, archetypes live as long as the EntityManager
  // Therefore I ensure that no UB ensues from using the IndexMap in a multithreaded environment
  archetypes: HashMap<ComponentBitmask<B>, Entities>,
  // Need this instead of self.entities.len() because later on will implement id recycling
  // So id allocation will become separate from self.entities.len()
  next_entity_id: u32
}

impl<const B: usize> EntityManager<B> {
  pub fn new() -> Self {
    let mut archetypes = HashMap::new();
    // Does ::new_empty() archetype NEED to exist by default? not really as far as I can tell
    // Keeping it for now because it makes self.create_entity() code simpler for testing
    archetypes.insert(ComponentBitmask::new_empty(), Arc::new(RwLock::new(Vec::new())));
    EntityManager { entities: Arc::new(DashMap::new()), archetypes, next_entity_id: 0 }
  }

  pub fn create_entity(&mut self) -> u32 {
    let id = self.next_entity_id;
    self.next_entity_id += 1;
    self.entities.insert(id, ComponentBitmask::new_empty());
    self.archetypes.get_mut(&ComponentBitmask::new_empty()).unwrap().write().push(id);
    id
  }

  pub fn has_component(&self, entity: u32, component: u32) -> Result<bool, P1Error> {
    let bitmask = self.entities.get(&entity).ok_or(EntityError::NotFound)?;
    Ok(bitmask.check_bit(component as usize)?)
  }

  pub fn add_component(&mut self, entity: u32, component: u32) -> Result<(), P1Error> {
    // This variable is keeping an immutable borrow of self alive so we need to kill it manually
    let mut bitmask = self.entities.get_mut(&entity).ok_or(EntityError::NotFound)?;
    bitmask.add(component)?;
    let archetype = *bitmask;
    drop(bitmask);

    self.add_to_archetype_unchecked(entity, archetype);

    Ok(())
  }

  // Debugging purposes, should delete eventually
  pub fn print_components(&self, entity: u32) {
    println!("{}", self.entities.get(&entity).unwrap().value())
  }

  // Archetypes get created here if they do not already exist
  // should probably change to an Option
  // and implement the creation code in a separate method
  // as a result, internal use would become:
  // manager.get_archetype(archetype)
  //  .unwrap_or(manager.create_archetype(archetype))
  // probably not unwrap actually because want to have proper error handling
  pub fn get_archetype(&mut self, archetype: ComponentBitmask<B>) -> Arc<RwLock<Vec<u32>>> {
    if let Some(archetype) = self.archetypes.get(&archetype) {
      return Arc::clone(archetype)
    }

    let arc = Arc::clone(&self.entities);

    let mut entities = Vec::new();
    for item in arc.iter() {
      if archetype.contains(*item.value()) {
        entities.push(*item.key());
      }
    }

    let result = Arc::new(RwLock::new(entities));
    self.archetypes.insert(archetype, Arc::clone(&result));
    

    result
  }

  // Archetype may or may not exist
  // entity uid is only added if it exists
  //
  // # Safety
  //
  // The caller must ensure that the entity uid is a valid entity uid
  //
  // Use of an invalid entity uid may result in unintented side effects later down the line
  // such as a MismatchedComponentBitmask or a NotFound entity error
  //
  fn add_to_archetype_unchecked(&mut self, entity: u32, archetype: ComponentBitmask<B>) {
    if let Some(entities) = self.archetypes.get_mut(&archetype) {
      entities.write().push(entity);
    }
  }
}
