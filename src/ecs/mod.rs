mod component;
mod entities;
mod query;
mod error;
mod archetype;

pub use error::*;
pub use component::Component;
pub use query::Query;

pub(crate) use component::{ComponentManager, ComponentContainer};
pub(crate) use entities::EntityManager;
pub(crate) use archetype::{Archetype, ArchetypeManager};
pub(crate) use query::QueryData;
