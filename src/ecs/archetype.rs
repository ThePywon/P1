use std::any::TypeId;
use std::mem::transmute;

use dashmap::mapref::one::{Ref, RefMut};
use dashmap::{DashMap, Entry};

pub(crate) struct Archetype {
  c_ids: Vec<TypeId>,
  entities: Vec<u32>
}

impl Archetype {
  pub fn new(c_ids: Vec<TypeId>, entities: Vec<u32>) -> Self {
    Self { c_ids, entities }
  }

  pub fn entities(&self) -> &Vec<u32> { &self.entities }
  pub fn entities_mut(&mut self) -> &mut Vec<u32> { &mut self.entities }

  pub fn id_from_c_ids(c_ids: &Vec<TypeId>) -> u128 {
    c_ids.iter().map(|c_id|
      // TypeIds can be safely transmuted to u128
      unsafe { transmute::<TypeId, u128>(*c_id) }
    ).fold(0u128, |a, b| a.saturating_add(b))
  }
}


pub(crate) struct ArchetypeManager(DashMap<u128, Archetype>);

impl ArchetypeManager {
  pub fn new() -> Self {
    Self ( DashMap::new() )
  }

  pub fn get(&self, archetype: u128) -> Option<Ref<'_, u128, Archetype>> {
    self.0.get(&archetype)
  }
  pub fn get_mut(&mut self, archetype: u128) -> Option<RefMut<'_, u128, Archetype>> {
    self.0.get_mut(&archetype)
  }

  pub fn insert(&mut self, c_ids: Vec<TypeId>, entities: Vec<u32>) -> RefMut<'_, u128, Archetype> {
    match self.0.entry(Archetype::id_from_c_ids(&c_ids)) {
      Entry::Vacant(entry) => entry.insert(Archetype::new(c_ids, entities)),
      Entry::Occupied(entry) => entry.into_ref()
    }
  }
}
