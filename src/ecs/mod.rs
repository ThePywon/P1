mod component;
mod entity;
mod query;
mod archetype;

pub use component::Component;
pub use query::Query;

pub(crate) use component::ComponentManager;
pub(crate) use entity::EntityManager;
pub(crate) use archetype::{Archetype, ArchetypeManager};
pub(crate) use query::QueryData;
