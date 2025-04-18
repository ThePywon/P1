use std::ffi::CString;

pub(super) fn create_whitespace_cstring_with_len(len: usize) -> CString {
  let mut buffer: Vec<u8> = Vec::with_capacity(len + 1);
  buffer.extend([b' '].iter().cycle().take(len));
  unsafe { CString::from_vec_unchecked(buffer) }
}

use std::any::{Any, TypeId};
use std::collections::{
  hash_map::{Values, ValuesMut},
  HashMap,
};
use std::hash::Hash;

use crate::error::UtilityContainerError;

pub struct SyncBox(Box<dyn Any>);

impl SyncBox {
  pub fn new<T: Send + Sync + Any>(data: T) -> Self {
    Self(Box::new(data))
  }

  pub fn cast_ref<T: Send + Sync + Any>(&self) -> Option<&T> {
    self.0.downcast_ref::<T>()
  }
  pub fn cast_mut<T: Send + Sync + Any>(&mut self) -> Option<&mut T> {
    self.0.downcast_mut::<T>()
  }
}

// This is safe because the compiler will enforce the safety restriction thanks to <T: Send + Sync + Any>
//
// SAFETY:
// Must ensure that the erased type implements Send + Sync
unsafe impl Send for SyncBox {}
unsafe impl Sync for SyncBox {}

pub struct ErasedMapContainer<K: Eq + Hash> {
  type_id: TypeId,
  // Using HashMap here instead of DashMap because downcasting gets the inner value
  // Without regards to the lock, thus eliminating any potential thread safety we could've gotten there
  // To make this thread safe, I must ensure that the ErasedMapContainer is only ever accessed from a RwLock or the likes
  ptrs: HashMap<K, SyncBox>,
}

impl<K: Eq + Hash> ErasedMapContainer<K> {
  pub fn new<T: Send + Sync + Any>() -> Self {
    Self {
      type_id: TypeId::of::<T>(),
      ptrs: HashMap::new(),
    }
  }

  pub fn is<T: Send + Sync + Any>(&self) -> bool {
    TypeId::of::<T>() == self.type_id
  }

  pub fn insert<T: Send + Sync + Any>(
    &mut self,
    key: K,
    data: T,
  ) -> Result<(), UtilityContainerError> {
    if !self.is::<T>() {
      return Err(UtilityContainerError::MismatchedTypeId(
        self.type_id,
        TypeId::of::<T>(),
      ));
    } else if self.ptrs.contains_key(&key) {
      return Err(UtilityContainerError::EntryOccupied);
    }

    self.ptrs.insert(key, SyncBox::new(data));

    Ok(())
  }

  pub fn get<T: Send + Sync + Any>(&self, key: &K) -> Option<&T> {
    self.ptrs.get(key).and_then(|ptr| ptr.cast_ref::<T>())
  }

  pub fn get_mut<T: Send + Sync + Any>(&mut self, key: &K) -> Option<&mut T> {
    self.ptrs.get_mut(key).and_then(|ptr| ptr.cast_mut::<T>())
  }

  pub fn iter(&self) -> Values<'_, K, SyncBox> {
    self.ptrs.values()
  }

  pub fn iter_mut(&mut self) -> ValuesMut<'_, K, SyncBox> {
    self.ptrs.values_mut()
  }
}
