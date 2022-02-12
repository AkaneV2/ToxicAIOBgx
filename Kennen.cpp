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

	void qlogic();
	void wlogic();
	void elogic();
	void rlogic();

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 1050);
		q->set_skillshot(0.175, 0, 1650, { collisionable_objects::minions, collisionable_objects::yasuo_wall, collisionable_objects::heroes }, skillshot_type::skillshot_line);
		w = plugin_sdk->register_spell(spellslot::w, 750);
		e = plugin_sdk->register_spell(spellslot::e, 0);
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
				combo::usee = combo->add_checkbox("UseE", "Use E", true);
				combo::usee->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
				combo::usew = combo->add_checkbox("UseR", "Use R", true);
				combo::usew->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
				auto rconfig = combo->add_tab("comboR", "R Settings");
				{
					combo::r_min_enemys = rconfig->add_slider("Rene", "Min R enemys in R range to use R", 2, 1, 5);
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

			auto draw = main_tab->add_tab("drawing", "Draw Settings");
			draw::drawq = draw->add_checkbox("drawQ", "Draw Q Range", true);
			draw::drawq->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			draw::draww = draw->add_checkbox("drawW", "Draw W Range", true);
			draw::draww->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			draw::drawe = draw->add_checkbox("drawE", "Draw E Range", true);
			draw::drawe->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			draw::drawr = draw->add_checkbox("drawR", "Draw R Range", true);
			draw::drawr->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
		}

		event_handler<events::on_update>::add_callback(on_update);
	}

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		event_handler<events::on_update>::remove_handler(on_update);

		menu->delete_tab("ta_Kennen");
	}

	void on_update()
	{
		if (myhero->is_dead())
		{
			return;
		}
		
	}
}