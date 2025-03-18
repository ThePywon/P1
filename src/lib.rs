use std::thread;
use std::sync::Arc;
//use std::time::{SystemTime, UNIX_EPOCH};

pub mod ecs;
pub mod rendering;
pub mod ui;
pub mod events;

use ecs::{Archetype, ArchetypeManager, Component, ComponentManager, EntityError, EntityManager, P1Error, Query, QueryData};
use parking_lot::RwLock;

/*#[macro_export]
macro_rules! init_engine {
  [$b:literal, $($i:ident),+] => {
    P1::<$b>::new(vec![$(std::any::TypeId::of::<$i>()),+])
  };
  [$($i:ident),+] => {
    P1::<{(0usize $(+ init_engine![@inner $i])+) / 8usize +
      if (0usize $(+ init_engine![@inner $i])+) % 8usize > 0usize { 1usize } else { 0usize }}>
      ::new(vec![$(std::any::TypeId::of::<$i>()),+])
  };
  [@inner $i:ident] => { 1usize }
}*/

pub struct P1 {
  entity_manager: EntityManager,
  archetype_manager: Arc<RwLock<ArchetypeManager>>,
  component_manager: Arc<RwLock<ComponentManager>>
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
      component_manager: Arc::new(RwLock::new(ComponentManager::new()))
    }
  }

  pub fn create_entity(&mut self) -> u32 {
    self.entity_manager.create_entity()
  }

  #[allow(dead_code)]
  fn has_component<C: Component>(&self, entity: u32) -> Result<bool, EntityError> {
    self.entity_manager.has_component::<C>(entity)
  }

  pub fn add_component<C: Component +  'static>(&mut self, entity: u32, component: C) -> Result<(), P1Error> {
    if self.has_component::<C>(entity)? {
      return Err(P1Error::ComponentExistsForEntity)
    }
    self.entity_manager.add_component::<C>(entity)?;

    self.component_manager.write().create_container::<C>()
      .insert(entity, component)
  }

  // Change archetypes to literally be a per system cache or if not, make sure they don't require a mutable access
  // Think of updating them with component changes though!
  // After second though, archetype initialization can be done outside system threads + readonly access can be requested every iteration instead of all time
  // Should make system struct to handle changes and iterations
  pub fn register_system<D: QueryData + 'static>(&mut self, callback: for<'a, 'b> fn(Query<'a, 'b, D>)) {
    let c_ids = D::component_ids();
    let archetype_id = Archetype::id_from_c_ids(&c_ids);
    let entities = self.entity_manager.get_archetype(&c_ids);
    self.archetype_manager.write().insert(c_ids, entities);

    // This is so I don't end up doing a 'static self borrow
    let archetype_manager = self.archetype_manager.clone();
    let component_manager = self.component_manager.clone();

    thread::spawn(move || {
      // Need event ticks soon
      loop {
        let lock = component_manager.read();
        let mut components: Vec<_> = archetype_manager.read().get(archetype_id).unwrap().entities().iter().map(|entity| {
          D::fetch(&lock, entity)
        }).collect();
        (callback)(Query::<D>::new(&mut components));
      }
    });
  }
}
