mod events;

use color_eyre::Result;
use color_eyre::eyre::Ok;
use crossterm::event::{self, KeyCode};
use ratatui::Frame;
use ratatui::style::{Color, Style, Stylize};
use ratatui::text::{Line, Text};
use ratatui::widgets::calendar::Monthly;
use time::ext::NumericalDuration;
use time::{Date, Month, OffsetDateTime};

use crate::events::Events;

fn main() -> Result<()> {
	color_eyre::install()?;
	let mut terminal = ratatui::init();

	let mut selected_date = OffsetDateTime::now_local()?.date();

	terminal.clear()?;
	loop {
		terminal.draw(|frame| render(frame, selected_date))?;
		if let Some(key) = event::read()?.as_key_press_event() {
			match key.code {
				| KeyCode::Char('q') => break Ok(()),
				| KeyCode::Tab => selected_date = next_month(selected_date),
				| KeyCode::BackTab => selected_date = prev_month(selected_date),
				| KeyCode::Left => selected_date -= 1.days(),
				| KeyCode::Down => selected_date += 1.weeks(),
				| KeyCode::Up => selected_date -= 1.weeks(),
				| KeyCode::Right => selected_date += 1.days(),
				| _ => {},
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

fn render(
	frame: &mut Frame,
	date: Date,
) {
	let area = frame.area();

	let header = Text::from_iter([
		Line::from("Calendar".bold()),
		Line::from("<Q> Quit| <tab> Next Month | <shift + tab> Previous Month, <arrow keys> Move"),
		Line::from(format!("Current date: {}", date)),
	]);

	frame.render_widget(header.centered(), area);

	let events = Events::new(date).add_selected();

	let calendar = Monthly::new(date, events.get_store())
		.default_style(Style::new().bold().bg(Color::Rgb(50, 50, 50)))
		.show_weekdays_header(Style::default())
		.show_surrounding(Style::new().dim());

	frame.render_widget(calendar, area);
}
