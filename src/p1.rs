use std::sync::atomic::{AtomicBool, Ordering};
use std::thread::{self, JoinHandle};
use std::sync::Arc;
use std::collections::HashSet;

use crate::ecs::{Archetype, ArchetypeManager, Component, ComponentManager, EntityManager, Query, QueryData};
use crate::error::{DataError, SystemError};
use parking_lot::RwLock;

pub struct P1 {
  entity_manager: EntityManager,
  archetype_manager: Arc<RwLock<ArchetypeManager>>,
  component_manager: Arc<RwLock<ComponentManager>>,
  // Systems need to exist soon and hold the handle
  thread_handles: Vec<JoinHandle<()>>,
  is_alive: Arc<AtomicBool>
}

impl P1 {
  pub fn new(/*components: Vec<TypeId>*/) -> Self {
    /*let timer = Arc::new(Mutex::new(0u32));
    let clone = timer.clone();
    let start = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .expect("Time went backwards");
    std::thread::spawn(move || {
      loop {
        let mut current = clone.lock().unwrap();
        let since_the_epoch = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .expect("Time went backwards");
        *current = (since_the_epoch.as_millis() - start.as_millis()) as u32;
        //dbg!(*current);
      }
    });*/
    P1 {
      entity_manager: EntityManager::new(),
      archetype_manager: Arc::new(RwLock::new(ArchetypeManager::new())),
      component_manager: Arc::new(RwLock::new(ComponentManager::new())),
      thread_handles: Vec::new(),
      is_alive: Arc::new(AtomicBool::new(true))
    }
  }

  pub fn create_entity(&mut self) -> u32 {
    self.entity_manager.create_entity()
  }

  pub fn has_component<C: Component>(&self, entity: u32) -> Result<bool, DataError> {
    self.entity_manager.has_component::<C>(entity)
  }

  pub fn add_component<C: Component +  'static>(&mut self, entity: u32, component: C) -> Result<(), DataError> {
    if self.has_component::<C>(entity)? {
      return Err(DataError::ComponentExistsForEntity)
    }
    self.entity_manager.add_component::<C>(entity)?;

    self.component_manager.write().create_container::<C>()
      .insert(entity, component).map_err(|e| e.into())
  }

  // Change archetypes to literally be a per system cache or if not, make sure they don't require a mutable access
  // Think of updating them with component changes though!
  // After second though, archetype initialization can be done outside system threads + readonly access can be requested every iteration instead of all time
  // Should make system struct to handle changes and iterations
  pub fn register_system<D: QueryData + 'static>(&mut self, callback: for<'a, 'b> fn(Query<'a, 'b, D>)) -> Result<(), SystemError> {
    let c_ids = D::component_ids();

    let mut unique = HashSet::new();

    if !c_ids.clone().into_iter().all(move |x| unique.insert(x)) {
      return Err(SystemError::QueryDeadlock)
    }

    let archetype_id = Archetype::id_from_c_ids(&c_ids);
    let entities = self.entity_manager.get_archetype(&c_ids);
    self.archetype_manager.write().insert(c_ids, entities);

    // This is so I don't end up doing a 'static self borrow
    let archetype_manager = self.archetype_manager.clone();
    let component_manager = self.component_manager.clone();
    let state = self.is_alive.clone();

    let t = thread::spawn(move || {
      // Need event ticks soon
      while state.load(Ordering::Relaxed) {
        let lock = component_manager.read();
        let mut components: Vec<_> = archetype_manager.read().get(archetype_id).unwrap().entities().iter().map(|entity| {
          D::fetch(&lock, entity).unwrap()
        }).collect();
        (callback)(Query::<D>::new(&mut components));
      }
    });

    self.thread_handles.push(t);

    Ok(())
  }
}

impl Drop for P1 {
  fn drop(&mut self) {
    self.is_alive.store(false, Ordering::Relaxed);
    for handle in self.thread_handles.drain(..) {
      handle.join().unwrap();
    }
  }
}

#[cfg(test)]
mod tests {
  use super::{P1, Component, Query};
  use crate::macros::Component;
  
  #[test]
  fn entity_creation() {
    let mut engine = P1::new();
    let entity_a = engine.create_entity();
    let entity_b = engine.create_entity();
    let entity_c = engine.create_entity();
    assert_eq!(entity_a, 0);
    assert_eq!(entity_b, 1);
    assert_eq!(entity_c, 2);
  }

  #[derive(Component)]
  struct TestComponentA();
  #[derive(Component)]
  struct TestComponentB();
  #[derive(Component)]
  struct TestComponentC();

  #[test]
  fn assigning_components() {
    let mut engine = P1::new();
    let entity = engine.create_entity();
    engine.add_component(entity, TestComponentA {}).unwrap();
    engine.add_component(entity, TestComponentB {}).unwrap();
    engine.add_component(entity, TestComponentC {}).unwrap();
    assert!(engine.has_component::<TestComponentA>(entity).unwrap());
    assert!(engine.has_component::<TestComponentB>(entity).unwrap());
    assert!(engine.has_component::<TestComponentC>(entity).unwrap());
  }

  #[test]
  #[should_panic(expected = "Cannot attach component to entity because a component of that type is already attached.")]
  fn assigning_preexisting_component() {
    let mut engine = P1::new();
    let entity = engine.create_entity();
    engine.add_component(entity, TestComponentA {}).unwrap();
    if let Err(e) = engine.add_component(entity, TestComponentA {}) {
      panic!("{}", e);
    }
  }

  #[test]
  #[should_panic(expected = "No entities with the provided id was found.")]
  fn non_existant_entity() {
    let mut engine = P1::new();
    // No entities were created, any id is invalid
    if let Err(e) = engine.add_component(0, TestComponentA {}) {
      panic!("{}", e);
    }
  }

  #[test]
  #[should_panic(expected = "Not all query items in system were unique.")]
  fn query_deadlock() {
    let mut engine = P1::new();

    let system = |_: Query<(&TestComponentA, &TestComponentA)>| {};

    if let Err(e) = engine.register_system(system) {
      panic!("{}", e);
    }
  }
}
