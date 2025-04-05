use super::Tick;
use crate::error::EventError;

use std::cmp::Ordering;
use std::collections::hash_map::Entry;
use std::collections::HashMap;
use std::sync::Arc;
use std::ops::Add;
use std::any::{Any, TypeId};
use std::hash::BuildHasherDefault;

use chrono::TimeDelta;
use parking_lot::RwLock;
use rustc_hash::FxHasher;



pub trait EventData: Send + Sync + Any {
  type Item: Clone + Copy;
  
  fn get_item(tick: &Tick/* , command_managers: _ */) -> Self::Item;
}

pub trait EventListener: Send + Sync + Any {
  fn check(&self, other: &Tick) -> bool;

  fn emit(&self);
}

pub struct SimpleListener(Arc<RwLock<Tick>>);

impl SimpleListener {
  pub fn new() -> Self {
    Self( Arc::new(RwLock::new(Tick::new())) )
  }
}
impl EventListener for SimpleListener {
  fn check(&self, other: &Tick) -> bool {
    self.0.read().cmp(other) == Ordering::Greater
  }

  fn emit(&self) {
    self.0.write().touch();
  }
}

pub struct IntervalListener(Arc<RwLock<Tick>>, u32);

impl IntervalListener {
  pub fn new(interval: u32) -> Self {
    Self( Arc::new(RwLock::new(Tick::new())), interval )
  }

  fn update(&self) {
    let check = self.0.read().add(self.1).cmp(&Tick::new());
    if check == Ordering::Less || check == Ordering::Equal {
      self.0.write().touch();
    }
  }
}
impl EventListener for IntervalListener {
  fn check(&self, other: &Tick) -> bool {
    self.update();

    self.0.read().cmp(other) == Ordering::Greater
  }

  fn emit(&self) {
    self.0.write().touch();
  }
}

pub struct Event<E: EventData>(E::Item);

impl<E: EventData> Event<E> {
  pub fn new(data: E::Item) -> Self {
    Self( data )
  }

  pub fn get_data(&self) -> E::Item { self.0 }
}


pub struct EventManager(HashMap<TypeId, Arc<RwLock<Box<dyn EventListener>>>, BuildHasherDefault<FxHasher>>);

impl EventManager {
  pub fn new() -> Self {
    Self( HashMap::with_hasher(BuildHasherDefault::default()) )
  }

  pub fn register_listener<E: EventData, L: EventListener>(&mut self, listener: L) -> Result<(), EventError> {
    match self.0.entry(TypeId::of::<E>()) {
      Entry::Vacant(entry) => { entry.insert(Arc::new(RwLock::new(Box::new(listener)))); },
      Entry::Occupied(_) => { return Err(EventError::EventAlreadyRegistered(TypeId::of::<E>())) }
    }

    Ok(())
  }

  pub fn check<E: EventData>(&self, tick: &Tick) -> Result<bool, EventError> {
    let key = TypeId::of::<E>();
    Ok(self.0.get(&key).ok_or(EventError::EventNotFound(key))?.read().check(tick))
  }

  pub fn emit<E: EventData>(&self) -> Result<(), EventError> {
    let key = TypeId::of::<E>();
    Ok(self.0.get(&key).ok_or(EventError::EventNotFound(key))?.read().emit())
  }
}

// To deal with inputs will require a proper input manager
// + some input calculations will require deltas
// + inputs will need to be cached alongside their ticks in order to accurately reply to threads

// Event managers will need to sort each events in their own containers each with a different key type and store listeners
// They will store the default and user registered events into the engine
// Command managers will store command data and events have access to it in order to instanciate their respective items

// Need command managers and a type-erased parent container for them
// Command managers will need a type for the commands
// + will store the command alongside the tick of when it was requested
// Might need to calculate the oldest system tick in order to do garbage collection on commands


// !!! Event identifiers !!!


// Type erased Event Listener here
// No need for casting because data is completely separate and stored in command managers
// Data is fetched from EventData trait
// Then fed back into a Event<EventData> object
// Event data could be implemented on a struct named Update for example
// Need to figure out how to give those impls a String key back to the listener(s) it is trying to get (maybe TypeId instead?)
// Listeners will need to be instantiated manually from an enum match statement... or externally!
// Multi-listeners soon? How? Easy, keep storing them the same way, just change the EventData to handle variadics
//pub(crate) struct EventManager(DashMap<String, Box<dyn EventListener>>);

// Need to change EventListener to EventData which will only have the Self::Item and .fetch()
// Type-Erased Container (utility::ErasedMapContainer : K = String) -cast_to-> EventListener object (some struct with ::<EventData>, will have the .check() and .emit())

/*
fn system(mut query: Query<(&mut A, &B)>, event: engine_events::Update) {
  let delta = event.delta();
}
*/

/*
fn system(mut query: Query<(&mut A, &B)>, event: engine_events::KeyDown) {
  let delta = event.key();
}
*/
