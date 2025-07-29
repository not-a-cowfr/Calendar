use color_eyre::Result;
use color_eyre::eyre::Ok;
use crossterm::event::{self, KeyCode};
use ratatui::Frame;
use ratatui::style::{Color, Modifier, Style, Stylize};
use ratatui::text::{Line, Text};
use ratatui::widgets::calendar::{CalendarEventStore, Monthly};
use time::ext::NumericalDuration;
use time::{Date, Month, OffsetDateTime};

fn main() -> Result<()> {
    color_eyre::install()?;
    let mut terminal = ratatui::init();

    let mut selected_date = OffsetDateTime::now_local()?.date();

    terminal.clear()?;
    loop {
        terminal.draw(|frame| render(frame, selected_date))?;
        if let Some(key) = event::read()?.as_key_press_event() {
            match key.code {
                KeyCode::Char('q') => break Ok(()),
                KeyCode::Tab => selected_date = next_month(selected_date),
                KeyCode::BackTab => selected_date = prev_month(selected_date),
                KeyCode::Left => selected_date -= 1.days(),
                KeyCode::Down => selected_date += 1.weeks(),
                KeyCode::Up => selected_date -= 1.weeks(),
                KeyCode::Right => selected_date += 1.days(),
                _ => {}
            }
        }
    }
}

fn next_month(date: Date) -> Date {
    if date.month() == Month::December {
        date.replace_month(Month::January)
            .unwrap()
            .replace_year(date.year() + 1)
            .unwrap()
    } else {
        date.replace_month(date.month().next()).unwrap()
    }
}

fn prev_month(date: Date) -> Date {
    if date.month() == Month::January {
        date.replace_month(Month::December)
            .unwrap()
            .replace_year(date.year() - 1)
            .unwrap()
    } else {
        date.replace_month(date.month().previous()).unwrap()
    }
}

fn render(frame: &mut Frame, date: Date) {
    let area = frame.area();

    let header = Text::from_iter([
        Line::from("Calendar".bold()),
        Line::from("<Q> Quit| <tab> Next Month | <shift + tab> Previous Month, <arrow keys> Move"),
        Line::from(format!("Current date: {}", date)),
    ]);

    frame.render_widget(header.centered(), area);

    let calendar = Monthly::new(date, events(date).unwrap())
        .default_style(Style::new().bold().bg(Color::Rgb(50, 50, 50)))
        .show_weekdays_header(Style::default())
        .show_surrounding(Style::new().dim());

    frame.render_widget(calendar, area);
}

pub struct EventData {
    name: String,
    date: Date,
    style: Style,
}

pub struct Events(Vec<EventData>);

impl Events {
    pub fn new() -> Self {
        Events(Vec::new())
    }

    pub fn get_store(&self) -> CalendarEventStore {
        let mut list = CalendarEventStore::today(
            Style::default()
                .add_modifier(Modifier::BOLD)
                .bg(Color::Blue),
        );
        for data in &self.0 {
            list.add(data.date, data.style);
        }
        list
    }

    pub fn get(&self) -> &Vec<EventData> {
        &self.0
    }

    pub fn add(&mut self, data: EventData) {
        self.0.push(data);
    }
}

fn events(selected_date: Date) -> Result<CalendarEventStore> {
    const SELECTED: Style = Style::new()
        .fg(Color::White)
        .bg(Color::Red)
        .add_modifier(Modifier::BOLD);

    let mut list = CalendarEventStore::today(
        Style::default()
            .add_modifier(Modifier::BOLD)
            .bg(Color::Blue),
    );

    list.add(selected_date, SELECTED);

    Ok(list)
}
