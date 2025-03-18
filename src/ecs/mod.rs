mod component;
mod entities;
mod query;
mod error;
mod archetype;

pub use error::{P1Error, EntityError, ComponentError};
pub use component::Component;
pub use query::{Query, QueryData};

pub(crate) use component::{ComponentManager, ComponentContainer};
pub(crate) use entities::EntityManager;
pub(crate) use archetype::{Archetype, ArchetypeManager};
