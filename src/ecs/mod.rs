mod archetype;
mod component;
mod entity;
mod query;

pub use component::Component;
pub use query::Query;

pub(crate) use archetype::{Archetype, ArchetypeManager};
pub(crate) use component::ComponentManager;
pub(crate) use entity::EntityManager;
pub(crate) use query::QueryData;
