#include "../ToxicAIO/plugin_sdk/plugin_sdk.hpp"
#include "kennen.h"

namespace kennen
{
#define Q_COLOR (MAKE_COLOR (0, 255, 255, 255 ))
#define W_COLOR (MAKE_COLOR (122, 22, 222, 255))
#define E_COLOR (MAKE_COLOR (32, 181, 15, 255))
#define R_COLOR (MAKE_COLOR (204, 0, 0, 255))

	script_spell* q = nullptr;
	script_spell* w = nullptr;
	script_spell* e = nullptr;
	script_spell* r = nullptr;

	TreeTab* main_tab = nullptr;

	namespace combo
	{
		TreeEntry* useq = nullptr;
		TreeEntry* usew = nullptr;
		TreeEntry* usee = nullptr;
		TreeEntry* user = nullptr;
		TreeEntry* r_min_enemys = nullptr;
		TreeEntry* roverkill = nullptr;
	}

	namespace harass
	{
		TreeEntry* useqh = nullptr;
		TreeEntry* usewh = nullptr;
	}

	namespace laneclear
	{
		TreeEntry* spell_farm = nullptr;
		TreeEntry* useqc = nullptr;
		TreeEntry* usewc = nullptr;
	}

	namespace wmode
	{
		TreeEntry* wmode = nullptr;
	}

	namespace jungleclear
	{
		TreeEntry* useqj = nullptr;
		TreeEntry* usewj = nullptr;
		TreeEntry* useej = nullptr;
	}

	namespace draw
	{
		TreeEntry* drawq = nullptr;
		TreeEntry* draww = nullptr;
		TreeEntry* drawe = nullptr;
		TreeEntry* drawr = nullptr;
	}

	//handler
	void on_update();
	void on_draw();

	void qlogic();
	void wlogic();
	void elogic();
	void rlogic();

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 950);
		q->set_skillshot(0.175, 0, 1650, { collisionable_objects::minions, collisionable_objects::yasuo_wall, collisionable_objects::heroes }, skillshot_type::skillshot_line);
		w = plugin_sdk->register_spell(spellslot::w, 770);
		e = plugin_sdk->register_spell(spellslot::e, 250);
		r = plugin_sdk->register_spell(spellslot::r, 550);

		main_tab = menu->create_tab("ta_Kennen", "Kennen");
		main_tab->set_assigned_texture(myhero->get_square_icon_portrait());
		{
			auto combo = main_tab->add_tab("combo", "Combo Settings");
			{
				combo::useq = combo->add_checkbox("UseQ", "Use Q", true);
				combo::useq->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
				combo::usew = combo->add_checkbox("UseW", "Use W", true);
				combo::usew->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
				auto wconfig = combo->add_tab("comboW", "W Settings");
				{
					wmode::wmode = wconfig->add_combobox("kwmode", "W Mode", { {"Always",nullptr},{"Only stun",nullptr } }, 0);
				}
				combo::usee = combo->add_checkbox("UseE", "Use E (Better use it Manually)", false);
				combo::usee->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
				combo::user = combo->add_checkbox("UseR", "Use R", true);
				combo::user->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
				auto rconfig = combo->add_tab("comboR", "R Settings");
				{
					combo::r_min_enemys = rconfig->add_slider("Rene", "Min R enemys in R range to use R", 2, 1, 5);
					combo::roverkill = rconfig->add_checkbox("rovver", "R Overkill protection", true);

				}

			}

			auto harass = main_tab->add_tab("harass", "Harass Settings");
			{
				harass::useqh = harass->add_checkbox("UseQH", "Use Q", true);
				harass::useqh->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
				harass::usewh = harass->add_checkbox("UseWH", "Use W", true);
				harass::usewh->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			}

			auto laneclear = main_tab->add_tab("lane", "Lane Clear Settings");
			{
				laneclear::spell_farm = laneclear->add_hotkey("LcT", "Enable Spell Farm", TreeHotkeyMode::Toggle, 'G', true);
				laneclear::useqc = laneclear->add_checkbox("LcQ", "Use Q", true);
				laneclear::useqc->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
				laneclear::usewc = laneclear->add_checkbox("LcW", "Use W", true);
				laneclear::usewc->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			}

			auto jungleclear = main_tab->add_tab("jungle", "Jungle Clear Settings");
			{
				jungleclear::useqj = jungleclear->add_checkbox("JcQ", "Use Q", true);
				jungleclear::useqj->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
				jungleclear::usewj = jungleclear->add_checkbox("JcW", "Use W", true);
				jungleclear::usewj->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
				jungleclear::useej = jungleclear->add_checkbox("JcE", "Use E", true);
				jungleclear::useej->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			}

			auto draw = main_tab->add_tab("drawing", "Draw Settings");
			{
				draw::drawq = draw->add_checkbox("drawQ", "Draw Q Range", true);
				draw::drawq->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
				draw::draww = draw->add_checkbox("drawW", "Draw W Range", true);
				draw::draww->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
				draw::drawr = draw->add_checkbox("drawR", "Draw R Range", true);
				draw::drawr->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
			}
		}

		event_handler<events::on_update>::add_callback(on_update);
		event_handler<events::on_draw>::add_callback(on_draw);
	}

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		event_handler<events::on_update>::remove_handler(on_update);
		event_handler<events::on_draw>::remove_handler(on_draw);

		menu->delete_tab("ta_Kennen");
	}

	float get_combo_damage(game_object_script enemy)
	{
		auto damage = 0.f;
		damage += q->is_ready() ? q->get_damage(enemy) : 0.f;
		damage += w->is_ready() ? w->get_damage(enemy) : 0.f;
		damage += e->is_ready() ? e->get_damage(enemy) : 0.f;
		damage += r->is_ready() ? r->get_damage(enemy) : 0.f;

		return (float)damage;
	}

	void on_update()
	{
		if (myhero->is_dead())
		{
			return;
		}

		if (orbwalker->combo_mode())
		{
			if (q->is_ready() && combo::useq->get_bool())
			{
				qlogic();
			}

			if (w->is_ready() && combo::usew->get_bool())
			{
				wlogic();
			}

			if (e->is_ready() && combo::usee->get_bool())
			{
				elogic();
			}

			if (r->is_ready() && combo::user->get_bool())
			{
				rlogic();
			}
		}

		if (orbwalker->harass())
		{
			if (q->is_ready() && harass::useqh->get_bool())
			{
				qlogic();
			}

			if (w->is_ready() && harass::usewh->get_bool())
			{
				wlogic();
			}
		}

		if (orbwalker->lane_clear_mode() && laneclear::spell_farm->get_bool())
		{
			auto lane_minions = entitylist->get_enemy_minions();

			auto junglemobs = entitylist->get_jugnle_mobs_minions();

			lane_minions.erase(std::remove_if(lane_minions.begin(), lane_minions.end(), [](game_object_script k)
				{
					return !k->is_valid_target(q->range());
				}), lane_minions.end());

			junglemobs.erase(std::remove_if(junglemobs.begin(), junglemobs.end(), [](game_object_script k)
				{
					return !k->is_valid_target(e->range());
				}), junglemobs.end());

			std::sort(lane_minions.begin(), lane_minions.end(), [](game_object_script a, game_object_script b)
				{
					return a->get_position().distance(myhero->get_position()) < b->get_position().distance(myhero->get_position());
				});

			std::sort(junglemobs.begin(), junglemobs.end(), [](game_object_script a, game_object_script b)
				{
					return a->get_max_health() > b->get_max_health();
				});

			if (!lane_minions.empty())
			{
				if (q->is_ready() && laneclear::useqc->get_bool())
				{
					if (q->cast(lane_minions.front()))
					{
						return;
					}
					if (q->cast(lane_minions.front()))
						return;
				}

				if (w->is_ready() && laneclear::usewc->get_bool())
				{
					if (lane_minions.front()->has_buff(buff_hash("kennenmarkofstorm")))
					{
						if (myhero->count_enemies_in_range(w->range()) == 0)
						{
							if (w->cast())
							{
								return;
							}
						}
						if (w->cast())
							return;
					}
				}
			}

			if (!junglemobs.empty())
			{
				if (q->is_ready() && jungleclear::useqj->get_bool())
				{
					if (q->cast(junglemobs.front()))
						return;
				}

				if (w->is_ready() && jungleclear::usewj->get_bool())
				{
					if (junglemobs.front()->has_buff(buff_hash("kennenmarkofstorm")))
					{
						if (w->cast())
							return;
					}
				}

				if (e->is_ready() && jungleclear::useej->get_bool())
				{
					if (e->cast(hud->get_hud_input_logic()->get_game_cursor_position()))
						return;
				}
			}
		}
	}

	void on_draw()
	{
		if (myhero->is_dead())
		{
			return;
		}

		//Draw Q
		if (q->is_ready() && draw::drawq->get_bool())
			draw_manager->add_circle(myhero->get_position(), q->range(), Q_COLOR);

		//Draw W
		if (draw::draww->get_bool())
			draw_manager->add_circle(myhero->get_position(), w->range(), W_COLOR);

		if (r->is_ready() && draw::drawr->get_bool())
			draw_manager->add_circle(myhero->get_position(), r->range(), R_COLOR);

		auto position = myhero->get_position();
		renderer->world_to_screen(position, position);
		auto spellclear = laneclear::spell_farm->get_bool();
		draw_manager->add_text_on_screen(position + vector(0, 40), (spellclear ? 0xFF00FF00 : 0xFF0000FF), 14, "FARM %s", (spellclear ? "On" : "Off"));
	}	

	void qlogic()
	{
		auto target = target_selector->get_target(q->range(), damage_type::magical);

		if (target != nullptr)
		{
			if (target->get_distance(myhero) <= q->range())
			{
				if (q->cast(target, hit_chance::high))				
				{
					return;
				}
			}
		}
	}

	void wlogic()
	{
		auto target = target_selector->get_target(w->range(), damage_type::magical);

		if (target != nullptr)
		{
			if (wmode::wmode->get_int() == 0)
			{
				if (target->get_distance(myhero) <= w->range() && target->is_ai_hero())
				{
					if (w->cast(target))
					{
						return;
					}
				}
			}
			else if (wmode::wmode->get_int() == 1)
			{
				if (target->get_distance(myhero) <= w->range())
				{
					if (target->get_buff_count(buff_hash("kennenmarkofstorm")) == 2 && target->is_ai_hero())
					{
						if (w->cast(target))
						{
							return;
						}
					}
				}
			}
		}
	}

	void elogic()
	{
		auto target = target_selector->get_target(e->range(), damage_type::magical);

		if (target != nullptr)
		{
			if (target->get_distance(myhero) <= e->range() && !myhero->get_buff(buff_hash("KennenLightningRush")))
			{
				if (e->cast(target))
				{
					return;
				}
			}
		}
	}

	void rlogic()
	{
		auto target = target_selector->get_target(r->range(), damage_type::magical);

		if (target != nullptr)
		{
			if (target->get_distance(myhero) <= r->range() && myhero->count_enemies_in_range(r->range()) >= combo::r_min_enemys->get_int())
			{
				if (r->cast())
				{
					return;
				}
			}
		}
	}
}