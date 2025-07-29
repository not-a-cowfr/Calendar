use std::collections::HashMap;

use ratatui::{
    style::{Color, Modifier, Style},
    widgets::calendar::CalendarEventStore,
};
use time::Date;

#[allow(non_snake_case)]
pub struct Events {
    pub data: HashMap<Date, (String, Style)>,
    pub SELECTED_DATE: Date,
}

impl Events {
    pub const SELECTED_STYLE: Style = Style::new()
        .fg(Color::White)
        .bg(Color::Red)
        .add_modifier(Modifier::BOLD);

    pub fn new(selected_date: Date) -> Self {
        Events {
            data: HashMap::new(),
            SELECTED_DATE: selected_date,
        }
    }

    pub fn add_selected(mut self) -> Self {
        self.data
            .insert(self.SELECTED_DATE, ("".to_string(), Self::SELECTED_STYLE));
        self
    }

    pub fn get_store(&self) -> CalendarEventStore {
        let mut list = CalendarEventStore::today(
            Style::default()
                .add_modifier(Modifier::BOLD)
                .bg(Color::Blue),
        );
        for (date, (_name, style)) in &self.data {
            list.add(*date, *style);
        }
        list
    }

    pub fn add(mut self, date: Date, name: String, style: Style) -> Self {
        self.data.insert(date, (name, style));
        self
    }
}
