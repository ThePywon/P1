use std::any::TypeId;
use std::fmt::Debug;
use std::marker::Sync;
use std::ops::{Deref, DerefMut};
use std::slice::{Iter, IterMut};

use dashmap::mapref::one::{MappedRef, MappedRefMut};
use parking_lot::RwLockReadGuard;

use super::{Component, ComponentManager};
use crate::utility::ErasedMapContainer;
use crate::error::{DataError, InternalDataError};

pub struct Query<'iterable, 'item: 'iterable, D: QueryData>(&'iterable mut Vec<D::Item<'item>>);

impl<'iterable, 'item: 'iterable, D: QueryData> Query<'iterable, 'item, D> {
  pub fn new(data: &'iterable mut Vec<D::Item<'item>>) -> Self {
    Self( data )
  }

  pub fn iter(&self) -> Iter<'_, D::Item<'item>> {
    self.0.iter()
  }
  pub fn iter_mut(&mut self) -> IterMut<'_, D::Item<'item>> {
    self.0.iter_mut()
  }
}

pub struct ComponentRef<'a, C: Component>(MappedRef<'a, TypeId, ErasedMapContainer<u32>, C>);

unsafe impl<C: Component> Send for ComponentRef<'_, C> {}
unsafe impl<C: Component> Sync for ComponentRef<'_, C> {}

impl<'a, C: Component> Deref for ComponentRef<'a, C> {
  type Target = MappedRef<'a, TypeId, ErasedMapContainer<u32>, C>;

  fn deref(&self) -> &Self::Target { &self.0 }
}
impl<C: Component> DerefMut for ComponentRef<'_, C> {
  fn deref_mut(&mut self) -> &mut Self::Target { &mut self.0 }
}
impl<C: Component + Debug> Debug for ComponentRef<'_, C> {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result { self.0.value().fmt(f) }
}

pub struct ComponentRefMut<'a, C: Component>(MappedRefMut<'a, TypeId, ErasedMapContainer<u32>, C>);

unsafe impl<C: Component> Send for ComponentRefMut<'_, C> {}
unsafe impl<C: Component> Sync for ComponentRefMut<'_, C> {}

impl<'a, C: Component> Deref for ComponentRefMut<'a, C> {
  type Target = MappedRefMut<'a, TypeId, ErasedMapContainer<u32>, C>;

  fn deref(&self) -> &Self::Target { &self.0 }
}
impl< C: Component> DerefMut for ComponentRefMut<'_, C> {
  fn deref_mut(&mut self) -> &mut Self::Target { &mut self.0 }
}
impl<C: Component + Debug> Debug for ComponentRefMut<'_, C> {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result { self.0.value().fmt(f) }
}


pub trait QueryData {
  type Item<'item>: Send + Sync;
  
  fn component_ids() -> Vec<TypeId>;
  #[allow(private_interfaces)]
  fn fetch<'item>(component_manager: &'item RwLockReadGuard<'_, ComponentManager>, entity: &u32) -> Result<Self::Item<'item>, DataError>;
}

impl<C: Component> QueryData for &C {
  type Item<'item> = ComponentRef<'item, C>;

  fn component_ids() -> Vec<TypeId> {
    vec![TypeId::of::<C>()]
  }
  #[allow(private_interfaces)]
  fn fetch<'item>(component_manager: &'item RwLockReadGuard<'_, ComponentManager>, entity: &u32) -> Result<Self::Item<'item>, DataError> {
    let container = component_manager.get_container::<C>().ok_or(InternalDataError::ContainerNotFound)?;
    Ok(ComponentRef(container.map(|c| c.get::<C>(&entity).unwrap())))
  }
}

impl<C: Component> QueryData for &mut C {
  type Item<'item> = ComponentRefMut<'item, C>;

  fn component_ids() -> Vec<TypeId> {
    vec![TypeId::of::<C>()]
  }
  #[allow(private_interfaces)]
  fn fetch<'item>(component_manager: &'item RwLockReadGuard<'_, ComponentManager>, entity: &u32) -> Result<Self::Item<'item>, DataError> {
    let container = component_manager.get_container_mut::<C>().ok_or(InternalDataError::ContainerNotFound)?;
    Ok(ComponentRefMut(container.map(|c| c.get_mut::<C>(&entity).unwrap())))
  }
}

impl QueryData for () {
  type Item<'item> = ();

  fn component_ids() -> Vec<TypeId> {
    Vec::new()
  }
  #[allow(private_interfaces)]
  fn fetch<'item>(_: &'item RwLockReadGuard<'_, ComponentManager>, _: &u32) -> Result<Self::Item<'item>, DataError> { Ok(()) }
}

macro_rules! impl_querydata {
  ($first:ident, $($inner: ident),*) => {
    impl<$first: QueryData, $($inner: QueryData),*> QueryData for ($first, $($inner),*) {
      type Item<'item> = ($first::Item<'item>, $($inner::Item<'item>),*);

      fn component_ids() -> Vec<TypeId> {
        vec![$first::component_ids(), $($inner::component_ids()),*]
          .iter().flatten().map(|x| *x).collect()
      }
      #[allow(private_interfaces)]
      fn fetch<'item>(component_manager: &'item RwLockReadGuard<'_, ComponentManager>, entity: &u32) -> Result<Self::Item<'item>, DataError> {
        Ok(($first::fetch(component_manager, entity)?, $($inner::fetch(component_manager, entity)?),*))
      }
    }

    impl_querydata!{$($inner),*}
  };
  ($inner:ident) => {
    impl<$inner: QueryData> QueryData for ($inner,) {
      type Item<'item> = $inner::Item<'item>;

      fn component_ids() -> Vec<TypeId> {
        $inner::component_ids()
      }
      #[allow(private_interfaces)]
      fn fetch<'item>(component_manager: &'item RwLockReadGuard<'_, ComponentManager>, entity: &u32) -> Result<Self::Item<'item>, DataError> {
        $inner::fetch(component_manager, entity)
      }
    }
  }
}

impl_querydata!{A, B, C, D, E, F, G, H}
