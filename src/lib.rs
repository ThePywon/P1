use std::cell::RefCell;
use std::collections::HashMap;
use std::any::{Any, TypeId};
use std::ops::DerefMut;
use std::sync::Arc;
use dashmap::DashMap;
//use std::time::{SystemTime, UNIX_EPOCH};

pub mod ecs;
pub mod rendering;
pub mod ui;
pub mod events;

use ecs::{ComponentError, ComponentIndexer, EntityError, EntityManager, P1Error, Query, QueryData, Mut, Ref};

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

type ComponentCell = Arc<RefCell<dyn Any>>;
type ComponentRow = Arc<DashMap<u32, ComponentCell>>;

pub struct P1<const B: usize> {
  entity_manager: EntityManager<B>,
  components: HashMap<u32, ComponentRow>,
  component_indexer: ComponentIndexer
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
    P1 { entity_manager: EntityManager::new(), components: HashMap::new(), component_indexer }
  }
  pub const fn size(&self) -> usize { B }

  pub fn create_entity(&mut self) -> u32 {
    self.entity_manager.create_entity()
  }

  pub fn has_component<C: 'static>(&self, entity: u32) -> Result<bool, P1Error> {
    let component = self.component_indexer.get_component_id(&TypeId::of::<C>())?;
    self.entity_manager.has_component(entity, component)
  }

  pub fn get_component<C: 'static>(&self, entity: u32) -> Result<ComponentCell, P1Error> {
    if !self.has_component::<C>(entity)? { return Err(ComponentError::NotFoundForEntity.into()) }

    let id = self.component_indexer.get_component_id(&TypeId::of::<C>())?;
    let b = self.components.get(&id).unwrap().get(&entity).unwrap();
    Ok(b.clone())
  }

  pub fn add_component<C: 'static>(&mut self, entity: u32, component: C) -> Result<(), P1Error> {
    let id = self.component_indexer.get_component_id(&TypeId::of::<C>())?;

    if self.has_component::<C>(entity)? {
      return Err(ComponentError::AlreadyExistsForEntity.into())
    }

    self.entity_manager.add_component(entity, id)?;

    if let Some(pool) = self.components.get_mut(&id) {
      pool.insert(entity, Arc::new(RefCell::new(component)));
    } else {
      let pool: ComponentRow = Arc::new(DashMap::new());
      pool.insert(entity, Arc::new(RefCell::new(component)));
      self.components.insert(id, pool);
    }

    Ok(())
  }

  pub fn register_system<'b, D: QueryData<Mut, Item<'b, dyn Any> = std::cell::RefMut<'b, dyn Any>>>(&mut self, callback: for<'a> fn(Query<'a, Mut, D>)) -> Result<(), P1Error> {
    println!("{}", D::bitmask::<B>(&self.component_indexer)?);
    let entities = self.entity_manager.get_archetype(D::bitmask(&self.component_indexer)?);

    let mut components = Vec::new();

    for id in entities.read().iter() {
      dbg!(id);
      components.push(self.get_component::<D::Item<'_, D>>(*id).map_err(|e| { match e {
        P1Error::Component(ComponentError::NotFoundForEntity) => P1Error::Entity(EntityError::MismatchedComponentBitmask),
        _ => e
      }})?);
    }

    let mut something: Vec<_> = components.iter().map(|c| {c.borrow_mut()}).collect();
    //let data = something.iter_mut().map(|c| D::try_as_mut(c.downcast_mut::<D::Item<'_, D>>().unwrap()).unwrap()).collect();

    (callback)(Query::new(something));
    

    Ok(())
  }

  pub fn test(&self, entity: u32) {
    self.entity_manager.print_components(entity);
  }
}
