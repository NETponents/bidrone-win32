using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;

using bidrone_win32.ScreenRotator;

namespace bidrone_win32
{
    namespace screens
    {
        class screen_mainmenu : Screen
        {
            public screen_mainmenu()
                : base("main_menu")
            {

            }
            public override void LoadContent(ref IServiceProvider isp)
            {
                base.LoadContent(ref isp);
            }
            public override void Draw(GraphicsDevice gd, ref SpriteBatch sb, ref GameTime gt)
            {
                base.Draw(gd, ref sb, ref gt);
            }
            public override void Update(GraphicsDevice gd, ref SpriteBatch sb, ref GameTime gt)
            {
                base.Update(gd, ref sb, ref gt);
            }
        }
    }
}
