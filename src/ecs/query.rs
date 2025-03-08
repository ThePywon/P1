use super::Component;


pub struct Query<'a, 'b, C: Component + 'static> {
  items: &'a mut Vec<&'b mut C>
}

impl<'a, 'b, C: Component + 'static> Query<'a, 'b, C> {
  pub fn new(items: &'a mut Vec<&'b mut C>) -> Self {
    Query { items }
  }

  pub fn iter_mut(&mut self) -> std::slice::IterMut<'_, &'b mut C> {
    return self.items.iter_mut();
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

/*pub trait QueryData<'b> {
  type Item<'a>;

  fn try_as_mut(item: &'b mut Self::Item<'b>) -> Option<Self::Item<'b>>;

  fn get_bitmask<const B: usize>(indexer: &ComponentIndexer) -> Option<ComponentBitmask<B>>;

  fn get_id(indexer: &ComponentIndexer) -> Option<u32>;
}

impl<'b, C: Component + 'static> QueryData<'b> for C {
  type Item<'a> = &'a mut C;

  fn try_as_mut(item: &'b mut Self::Item<'b>) -> Option<Self::Item<'b>> {
    Some(item)
  }

  fn get_bitmask<const B: usize>(indexer: &ComponentIndexer) -> Option<ComponentBitmask<B>> {
    None
  }

  fn get_id(indexer: &ComponentIndexer) -> Option<u32> {
    None
  }
}*/

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
