#define DEBUGHUD

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

namespace bidrone_win32
{
    namespace ScreenRotator
    {
        public class ScreenManager
        {
            Queue<string> destroyList;
            Dictionary<string, Screen> screenList;
            string currentScreen;

            public ScreenManager()
            {
                destroyList = new Queue<string>();
                screenList = new Dictionary<string, Screen>();
                currentScreen = "SCREEN::EMPTY";
            }
            public void Update(GraphicsDevice gd, ref SpriteBatch sb, ContentManager cm, ref GameTime gt)
            {
                if (ckSelectedScreen() == 1)
                {
                    throw new Exception("Engine error 0x01: no screen selected");
                }
                else
                {
                    while (destroyList.Count > 0)
                    {
                        screenList.Remove(destroyList.Dequeue());
                    }
                    screenList[currentScreen].Update(gd, ref sb, ref gt);
                }
            }
            public void Draw(GraphicsDevice gd, ref SpriteBatch sb, ContentManager cm, ref GameTime gt)
            {
                if (ckSelectedScreen() == 1)
                {
                    throw new Exception("Engine error 0x01: no screen selected");
                }
                else
                {
                    gd.Clear(Color.CornflowerBlue);
                    sb.Begin();
                    screenList[currentScreen].Draw(gd, ref sb, ref gt);
                    sb.End();
                }
            }
            public void loadScreens(IServiceProvider isp)
            {
                foreach (KeyValuePair<string, Screen> i in screenList)
                {
                    loadScreens(i.Key, isp);
                }
            }
            public void loadScreens(ref List<string> screens, IServiceProvider isp)
            {
                foreach (string i in screens)
                {
                    loadScreens(i, isp);
                }
            }
            public void loadScreens(string screen, IServiceProvider isp)
            {
                screenList[screen].LoadContent(ref isp);
            }
            public int markForDeletion(string screenName)
            {
                if (screenExists(screenName) == 0)
                {
                    destroyList.Enqueue(screenName);
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            public int screenExists(string screenName)
            {
                bool doesExist = false;
                foreach (KeyValuePair<string, Screen> i in screenList)
                {
                    if (i.Key == screenName)
                    {
                        doesExist = true;
                    }
                }
                if (doesExist)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            public void addScreen(string name, Screen screenObject)
            {
                screenList.Add(name, screenObject);
            }
            public void setScreen(string screen)
            {
                currentScreen = screen;
            }
            public string getScreen()
            {
                return currentScreen;
            }

            private int ckSelectedScreen()
            {
                if (currentScreen == "SCREEN::EMPTY")
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }
        public partial class Screen
        {
            private SpriteFont debugFont;
            private string screenName;
            public Screen(string screenname)
            {
                screenName = screenname;
            }
            public virtual void Update(GraphicsDevice gd, ref SpriteBatch sb, ref GameTime gt)
            {

            }
            public virtual void Draw(GraphicsDevice gd, ref SpriteBatch sb, ref GameTime gt)
            {
                sb.DrawString(debugFont, screenName, new Vector2(0, 0), Color.Yellow);
            }
            public virtual void LoadContent(ref IServiceProvider isp)
            {
                ContentManager cm = new ContentManager(isp, "Content");
                debugFont = cm.Load<SpriteFont>("Fonts/Engine/debugfont");
            }
        }
    }
}
