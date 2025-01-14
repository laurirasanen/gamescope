#include <linux/input-event-codes.h>

#include "shortcut.h"
#include "steamcompmgr_shared.hpp"
#include "main.hpp"

namespace gamescope
{
	ShortcutHandler::ShortcutHandler()
	{
		// default mappings
		m_binds = {
			{ KEY_F, GAMESCOPE_SHORTCUT_FULLSCREEN },
			{ KEY_N, GAMESCOPE_SHORTCUT_FILTER_NEAREST },
			{ KEY_B, GAMESCOPE_SHORTCUT_FILTER_LINEAR },
			{ KEY_U, GAMESCOPE_SHORTCUT_UPSCALE_FSR },
			{ KEY_Y, GAMESCOPE_SHORTCUT_UPSCALE_NIS },
			{ KEY_I, GAMESCOPE_SHORTCUT_UPSCALE_SHARPNESS_UP },
			{ KEY_O, GAMESCOPE_SHORTCUT_UPSCALE_SHARPNESS_DOWN },
			{ KEY_S, GAMESCOPE_SHORTCUT_SCREENSHOT },
			{ KEY_G, GAMESCOPE_SHORTCUT_KEYBOARD_GRAB }
		};
	}

	ShortcutHandler::~ShortcutHandler() {}

	void ShortcutHandler::Bind( uint32_t key, GamescopeShortcut shortcut )
	{
		Unbind( key );
		m_binds.insert( { key, shortcut } );
	}

	void ShortcutHandler::Unbind( uint32_t key )
	{
		m_binds.erase( key );
	}

	void ShortcutHandler::Unbind( GamescopeShortcut shortcut )
	{
		m_binds.erase( shortcut );
	}

	GamescopeShortcut ShortcutHandler::GetShortcut( uint32_t key )
	{
		if (auto it = m_binds.find( key ); it != m_binds.end())
		{
			return it->second;
		}

		return GAMESCOPE_SHORTCUT_NONE;
	}

	bool ShortcutHandler::HandleShortcut( GamescopeShortcut shortcut )
	{
		switch ( shortcut )
		{
			case GAMESCOPE_SHORTCUT_FILTER_NEAREST:
				g_wantedUpscaleFilter = GamescopeUpscaleFilter::PIXEL;
				break;
			case GAMESCOPE_SHORTCUT_FILTER_LINEAR:
				g_wantedUpscaleFilter = GamescopeUpscaleFilter::LINEAR;
				break;
			case GAMESCOPE_SHORTCUT_UPSCALE_FSR:
				g_wantedUpscaleFilter = (g_wantedUpscaleFilter == GamescopeUpscaleFilter::FSR) ?
					GamescopeUpscaleFilter::LINEAR : GamescopeUpscaleFilter::FSR;
				break;
			case GAMESCOPE_SHORTCUT_UPSCALE_NIS:
				g_wantedUpscaleFilter = (g_wantedUpscaleFilter == GamescopeUpscaleFilter::NIS) ? 
					GamescopeUpscaleFilter::LINEAR : GamescopeUpscaleFilter::NIS;
				break;
			case GAMESCOPE_SHORTCUT_UPSCALE_SHARPNESS_UP:
				g_upscaleFilterSharpness = std::min( 20, g_upscaleFilterSharpness + 1 );
				break;
			case GAMESCOPE_SHORTCUT_UPSCALE_SHARPNESS_DOWN:
				g_upscaleFilterSharpness = std::max( 0, g_upscaleFilterSharpness - 1 );
				break;
			case GAMESCOPE_SHORTCUT_SCREENSHOT:
				gamescope::CScreenshotManager::Get().TakeScreenshot( true );
				break;
			case GAMESCOPE_SHORTCUT_KEYBOARD_GRAB:
				g_bGrabbed = !g_bGrabbed;
				break;
			default:
				return false;
				break;
		}

		return true;
	}
}
