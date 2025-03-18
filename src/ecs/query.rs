use super::{Component, ComponentContainer, ComponentManager};
use std::any::TypeId;
use std::fmt::Debug;
use std::marker::Sync;
use std::hash::Hash;
use std::ops::{Deref, DerefMut};
use std::slice::{Iter, IterMut};
use dashmap::mapref::one::{MappedRef, MappedRefMut};
use parking_lot::RwLockReadGuard;

pub struct Query<'iterable, 'item: 'iterable, D: QueryData> {
  data: &'iterable mut Vec<D::Item<'item>>
}

impl<'iterable, 'item: 'iterable, D: QueryData> Query<'iterable, 'item, D> {
  pub fn new(data: &'iterable mut Vec<D::Item<'item>>) -> Self {
    Self { data }
  }

  pub fn iter(&self) -> Iter<'_, D::Item<'item>> {
    self.data.iter()
  }
  pub fn iter_mut(&mut self) -> IterMut<'_, D::Item<'item>> {
    self.data.iter_mut()
  }
}

pub struct MappedRefWrapper<'a, K: Hash + Eq, V, T: Debug>(MappedRef<'a, K, V, T>);

unsafe impl<'a, K: Hash + Eq, V, T: Debug> Send for MappedRefWrapper<'a, K, V, T> {}
unsafe impl<'a, K: Hash + Eq, V, T: Debug> Sync for MappedRefWrapper<'a, K, V, T> {}

impl<'a, K: Hash + Eq, V, T: Debug> Deref for MappedRefWrapper<'a, K, V, T> {
  type Target = MappedRef<'a, K, V, T>;

  fn deref(&self) -> &Self::Target { &self.0 }
}
impl<'a, K: Hash + Eq, V, T: Debug> DerefMut for MappedRefWrapper<'a, K, V, T> {
  fn deref_mut(&mut self) -> &mut Self::Target { &mut self.0 }
}
impl<'a, K: Hash + Eq, V, T: Debug> Debug for MappedRefWrapper<'a, K, V, T> {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result { self.0.value().fmt(f) }
}

pub struct MappedRefMutWrapper<'a, K: Hash + Eq, V, T: Debug>(MappedRefMut<'a, K, V, T>);

unsafe impl<'a, K: Hash + Eq, V, T: Debug> Send for MappedRefMutWrapper<'a, K, V, T> {}
unsafe impl<'a, K: Hash + Eq, V, T: Debug> Sync for MappedRefMutWrapper<'a, K, V, T> {}

impl<'a, K: Hash + Eq, V, T: Debug> Deref for MappedRefMutWrapper<'a, K, V, T> {
  type Target = MappedRefMut<'a, K, V, T>;

  fn deref(&self) -> &Self::Target { &self.0 }
}
impl<'a, K: Hash + Eq, V, T: Debug> DerefMut for MappedRefMutWrapper<'a, K, V, T> {
  fn deref_mut(&mut self) -> &mut Self::Target { &mut self.0 }
}
impl<'a, K: Hash + Eq, V, T: Debug> Debug for MappedRefMutWrapper<'a, K, V, T> {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result { self.0.value().fmt(f) }
}


pub trait QueryData {
  type Item<'item>: Send + Sync;
  
  fn component_ids() -> Vec<TypeId>;
  #[allow(private_interfaces)]
  fn fetch<'item>(component_manager: &'item RwLockReadGuard<'_, ComponentManager>, entity: &u32) -> Self::Item<'item>;
}

impl<C: Component> QueryData for &C {
  type Item<'item> = MappedRefWrapper<'item, TypeId, ComponentContainer, C>;

  fn component_ids() -> Vec<TypeId> {
    vec![TypeId::of::<C>()]
  }
  #[allow(private_interfaces)]
  fn fetch<'item>(component_manager: &'item RwLockReadGuard<'_, ComponentManager>, entity: &u32) -> Self::Item<'item> {
    let container = component_manager.get_container::<C>().unwrap();
    MappedRefWrapper(container.map(|c| c.get::<C>(&entity).unwrap()))
  }
}

impl<C: Component> QueryData for &mut C {
  type Item<'item> = MappedRefMutWrapper<'item, TypeId, ComponentContainer, C>;

  fn component_ids() -> Vec<TypeId> {
    vec![TypeId::of::<C>()]
  }
  #[allow(private_interfaces)]
  fn fetch<'item>(component_manager: &'item RwLockReadGuard<'_, ComponentManager>, entity: &u32) -> Self::Item<'item> {
    let container = component_manager.get_container_mut::<C>().unwrap();
    MappedRefMutWrapper(container.map(|c| c.get_mut::<C>(&entity).unwrap()))
  }
}

macro_rules! impl_querydata {
  ($first:ident, $($inner: ident),+) => {
    impl<$first: QueryData, $($inner: QueryData),+> QueryData for ($first, $($inner),+) {
      type Item<'item> = ($first::Item<'item>, $($inner::Item<'item>),*);

      fn component_ids() -> Vec<TypeId> {
        vec![$first::component_ids(), $($inner::component_ids()),*]
          .iter().flatten().map(|x| *x).collect()
      }

      #[allow(private_interfaces)]
      fn fetch<'item>(component_manager: &'item RwLockReadGuard<'_, ComponentManager>, entity: &u32) -> Self::Item<'item> {
        ($first::fetch(component_manager, entity), $($inner::fetch(component_manager, entity)),*)
      }
    }

    impl_querydata!{$($inner),+}
  };
  ($inner: ident) => {
    impl<$inner: QueryData> QueryData for ($inner,) {
      type Item<'item> = $inner::Item<'item>;

      fn component_ids() -> Vec<TypeId> {
        $inner::component_ids()
      }

      #[allow(private_interfaces)]
      fn fetch<'item>(component_manager: &'item RwLockReadGuard<'_, ComponentManager>, entity: &u32) -> Self::Item<'item> {
        $inner::fetch(component_manager, entity)
      }
    }
  }
}

impl_querydata!{A, B, C, D, E, F, G, H}
