use super::{Component, ComponentBitmask, ComponentError, ComponentIndexer};
use std::{any::{Any, TypeId}, cell::{RefCell, RefMut}, marker::PhantomData, ops::{Deref, DerefMut}, slice::IterMut, sync::Arc};

#[derive(Debug)]
pub struct Query<'a, A: Aliasing, D: QueryData<A> + 'static>/* where &'a I: IntoIterator<Item = D::Item>*/ {
  data: Vec<D::Item<'a, dyn Any>>,
  index: usize,
  marker: PhantomData<(A, D)>
}

impl<'a, A: Aliasing, D: QueryData<A>> Query<'a, A, D> {
  pub fn new(data: Vec<D::Item<'a, dyn Any>>) -> Self {
    Query { data, index: 0usize, marker: PhantomData }
  }
}
/*
impl<'a, D: QueryData> Iterator for Query<'a, D> {
  type Item = &'a mut D::Item;

  fn next(&mut self) -> Option<Self::Item> {
    if self.index < self.data.len() {
      return Some(self.data[self.index]);
    }

    self.index = 0;
    None
  }
}
*/

pub enum Mut {}
pub enum Ref {}

pub trait Aliasing {}

impl Aliasing for Mut {}
impl Aliasing for Ref {}

pub trait QueryData<A: Aliasing> {
  type Item<'a, I> where I: 'a + ?Sized;

  fn try_as_mut<'a, 'b, I>(item: &'a mut Self::Item<'b, I>) -> Option<Self::Item<'b, I>> where 'a: 'b {
    None
  }

  fn try_as_ref<'a, I>(item: Self::Item<'a, I>) -> Option<&'a I> {
    None
  }

  #[allow(private_interfaces)]
  fn bitmask<'a, const B: usize>(indexer: &'a ComponentIndexer) -> Result<ComponentBitmask<B>, ComponentError>;
}

impl<T: 'static + Component> QueryData<Mut> for T {
  type Item<'a, I> = RefMut<'a, I> where I: 'a + ?Sized;

  // Might wanna change to a specific struct/trait to handle the convertion
  #[allow(private_interfaces)]
  fn bitmask<'a, const B: usize>(indexer: &'a ComponentIndexer) -> Result<ComponentBitmask<B>, ComponentError> {
    let mut result = ComponentBitmask::new_empty();
    result.add(indexer.get_component_id(&TypeId::of::<T>())?)?;
    Ok(result)
  }
}

fn takes_iterator<'a, C: 'a + Component>(i: impl IntoIterator<Item = &'a mut C>) {
  for _ in i {

  }
}

fn returns_iterator<'a, C: 'a + Component>() -> impl IntoIterator<Item = &'a mut C> {
  Vec::new()
}

/*
  engine.register_system(|query: Query<(Option<&mut A>, &B, &C)>| {
    for element in &query {
      // readonly
    }

    for mut element in &mut query {
      // mutable
    }
  });
*/
