use std::any::TypeId;
use std::thread;
//use std::time::{SystemTime, UNIX_EPOCH};

pub mod ecs;
pub mod rendering;
pub mod ui;
pub mod events;

use ecs::{Component, ComponentBitmask, ComponentManager, ComponentError, ComponentIndexer, EntityManager, P1Error, Query};
use parking_lot::RwLock;

#[macro_export]
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
}

pub struct P1<const B: usize> {
  entity_manager: EntityManager<B>,
  component_indexer: ComponentIndexer,
  component_manager: RwLock<ComponentManager<B>>
}

impl<const B: usize> P1<B> {
  pub fn new(components: Vec<TypeId>) -> Self {
    let mut component_indexer = ComponentIndexer::new::<B>();
    // Do not wanna handle allocation error
    // it should only ever happen if the user is manually setting the allocation amount
    // in which case I want to throw an explicit error, it's an invalid state
    component_indexer.register_components(components).unwrap();

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
    P1 { entity_manager: EntityManager::new(), component_manager: RwLock::new(ComponentManager::new()), component_indexer }
  }
  pub const fn size(&self) -> usize { B }

  pub fn create_entity(&mut self) -> u32 {
    self.entity_manager.create_entity()
  }

  #[allow(dead_code)]
  fn has_component(&self, entity: u32, c_id: u32) -> Result<bool, P1Error> {
    self.entity_manager.has_component(entity, c_id)
  }

  pub fn get_component<C: Component +  'static>(&self, entity: u32) -> Result<ecs::ComponentCell, P1Error> {
    let component = self.component_indexer.get_component_id(&TypeId::of::<C>())?;
    if !self.has_component(entity, component)? { return Err(ComponentError::NotFoundForEntity.into()) }

    self.component_manager.read().get_component(entity, component).map_err(|e| e.into())
  }

  pub fn add_component<C: Component +  'static>(&mut self, entity: u32, component: C) -> Result<(), P1Error> {
    let c_id = self.component_indexer.get_component_id(&TypeId::of::<C>())?;

    if self.has_component(entity, c_id)? {
      return Err(ComponentError::AlreadyExistsForEntity.into())
    }
    self.entity_manager.add_component(entity, c_id)?;

    self.component_manager.write().add_component(entity, c_id, component).map_err(|e| e.into())
  }

  // Change archetypes to literally be a per system cache or if not, make sure they don't require a mutable access
  // Think of updating them with component changes though!
  // After second though, archetype initialization can be done outside system threads + readonly access can be requested every iteration instead of all time
  // Should make system struct to handle changes and iterations
  pub fn register_system<C: Component + 'static>(&mut self, callback: for<'a, 'b> fn(Query<'a, 'b, C>)) -> Result<(), P1Error> {
    let c_id = self.component_indexer.get_component_id(&TypeId::of::<C>()).unwrap();
    let mut bitmask = ComponentBitmask::new_empty();
    bitmask.add(c_id).unwrap();
    let entities = self.entity_manager.get_archetype(bitmask);

    let mut components = Vec::new();

    let component_manager = self.component_manager.read();
    
    for entity in entities.read().iter() {
      components.push(component_manager.get_component(*entity, c_id).map_err(<ComponentError as Into<P1Error>>::into).unwrap());
    }

    thread::spawn(move || {
      let mut mut_borrow: Vec<_> = components.iter().map(|c| {c.write().unwrap()}).collect();
      let mut data: Vec<_> = mut_borrow.iter_mut().map(|c| c.downcast_mut::<C>().ok_or(P1Error::Component(ComponentError::IllAllocated)).unwrap()).collect();

      loop {
        (callback)(Query::new(&mut data));
      }
    });
    

    Ok(())
  }

  pub fn test(&self, entity: u32) {
    self.entity_manager.print_components(entity);
  }
}
