#include <stdlib.h>
#include <common.h>
#include <command.h>
#include <user_setting.h>
#include <panel.h>

int do_backlight_debug(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int i = 0, j = 0, val = 0;
	unsigned int tmp_off = 0, tmp_len = 0;
	const char *cmd;
	unsigned char *tmp_buf;
	char *tmp_ptr;
	char *endp;

	if ( argc < 2 ) {
		goto usage;
	}

	cmd = argv[1];

	if ( strcmp ( cmd, "power" ) == 0 ) {
		if ( argc < 3 ) {
			goto usage;
		}
		cmd = argv[2];
		val = strtoul ( cmd, &endp, 10 );
		backlight_power_ctrl((char)val);
		printf("backlight: power %s\n", val ? "on" : "off");
		return 0;
	} else if ( strcmp ( cmd, "set" ) == 0 ) {
		if ( argc < 3 ) {
			goto usage;
		}
		cmd = argv[2];
		val = strtoul ( cmd, &endp, 10 );
		backlight_set_level(val);
		return 0;
	} else if ( strcmp ( cmd, "get" ) == 0 ) {
		if ( argc < 2 ) {
			goto usage;
		}
		val = backight_get_level();
		printf("backlight: level = %d\n", val);
		return 0;
	} else if ( strcmp ( cmd, "status" ) == 0 ) {
		if ( argc < 2 ) {
			goto usage;
		}
		/* to do */
		/* print backlight info */
		printf("to do\n");
		return 0;
	} else if ( strcmp ( cmd, "bypass" ) == 0 ) {
		/* to do */
		/* bypass backlight level adjust */
		printf("to do\n");
		return 0;
	} else if ( strcmp ( cmd, "pwm" ) == 0 ) {
		if ( argc < 4 ) {
			goto usage;
		}
		cmd = argv[2];
		if ( strcmp ( cmd, "duty" ) == 0 ) {
			/* to do */
			/* change pwm duty */
			printf("to do\n");
			return 0;
		} else if ( strcmp ( cmd, "freq" ) == 0 ) {
			/* to do */
			/* change pwm frequency */
			printf("to do\n");
			return 0;
		} else if ( strcmp ( cmd, "pol" ) == 0 ) {
			/* to do */
			/* change pwm polarity */
			printf("to do\n");
			return 0;
		}
		return 0;
	}

usage:
	return cmd_usage ( cmdtp );
}

int do_panel_debug(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int i = 0, j = 0, val = 0;
	int temp[4];
	unsigned int tmp_off = 0, tmp_len = 0;
	const char *cmd;
	unsigned char *tmp_buf;
	char *tmp_ptr;
	char *endp;

	if ( argc < 2 ) {
		goto usage;
	}

	cmd = argv[1];

	if ( strcmp ( cmd, "enable" ) == 0 ) {
		if ( argc < 3 ) {
			goto usage;
		}
		cmd = argv[2];
		val = strtoul ( cmd, &endp, 10 );
		if (val) {
			printf("panel: enable\n");
			panel_resume();
		} else {
			printf("panel: disable\n");
			panel_suspend();
		}
		return 0;
	} else if ( strcmp ( cmd, "test" ) == 0 ) {
		if ( argc < 3 ) {
			goto usage;
		}
		cmd = argv[2];
		val = strtoul ( cmd, &endp, 10 );
		/* to do */
		/* change to free_run mode, and show bist pattern */
		printf("to do\n");
		return 0;
	} else if ( strcmp ( cmd, "reset" ) == 0 ) {
		if ( argc < 2 ) {
			goto usage;
		}
		/* to do */
		/* reset panel */
		printf("to do\n");
		return 0;
	} else if ( strcmp ( cmd, "ss" ) == 0 ) {
		if ( argc < 3 ) {
			printf("lcd ss: %s(%d)\n", get_lcd_clk_ss(),
				panel_param->clk_ss_level);
			return 0;
		}
		cmd = argv[2];
		val = strtoul ( cmd, &endp, 10 );
		panel_param->clk_ss_level = val;
		set_lcd_clk_ss(val);
		return 0;
	} else if ( strcmp ( cmd, "phy" ) == 0 ) {
		if ( argc < 4 ) {
			printf("lcd phy: vswing=%d, preem=%d\n",
				panel_param->vx1_lvds_phy_vswing,
				panel_param->vx1_lvds_phy_preem);
			return 0;
		}
		cmd = argv[2];
		temp[0] = strtoul ( cmd, &endp, 10 );
		cmd = argv[3];
		temp[1] = strtoul ( cmd, &endp, 10 );
		lcd_phy_adjust(temp[0], temp[1]);
		return 0;
	} else if ( strcmp ( cmd, "lvds" ) == 0 ) {
		if ( argc < 6 ) {
			goto usage;
		}
		cmd = argv[2];
		temp[0] = strtoul ( cmd, &endp, 10 );
		cmd = argv[3];
		temp[1] = strtoul ( cmd, &endp, 10 );
		cmd = argv[4];
		temp[2] = strtoul ( cmd, &endp, 10 );
		cmd = argv[5];
		temp[3] = strtoul ( cmd, &endp, 10 );
		/* to do */
		/* set lvds config */
		printf("to do\n");
		return 0;
	} else if ( strcmp ( cmd, "vbyone" ) == 0 ) {
		if ( argc < 5 ) {
			goto usage;
		}
		cmd = argv[2];
		temp[0] = strtoul ( cmd, &endp, 10 );
		cmd = argv[3];
		temp[1] = strtoul ( cmd, &endp, 10 );
		cmd = argv[4];
		temp[2] = strtoul ( cmd, &endp, 10 );
		/* to do */
		/* set vbyone config */
		printf("to do\n");
		return 0;
	} else if ( strcmp ( cmd, "info" ) == 0 ) {
		if ( argc < 2 ) {
			goto usage;
		}
		/* to do */
		/* print panel info */
		printf("to do\n");
		return 0;
	} else if ( strcmp ( cmd, "reg" ) == 0 ) {
		if ( argc < 2 ) {
			goto usage;
		}
		/* to do */
		/* print panel reg */
		printf("to do\n");
		return 0;
	}
	 else if ( strcmp ( cmd, "dump" ) == 0 ) {
		if ( argc < 2 ) {
			goto usage;
		}
		/* to do */
		/* print panel info & reg */
		printf("to do\n");
		return 0;
	}
	 else if ( strcmp ( cmd, "debug_print" ) == 0 ) {
		if ( argc < 3 ) {
			goto usage;
		}
		cmd = argv[2];
		val = strtoul ( cmd, &endp, 10 );
		/* to do */
		/* set panel debug print */
		printf("to do\n");
		return 0;
	}

usage:
	return cmd_usage ( cmdtp );
}

