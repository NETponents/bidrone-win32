using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace bidrone
{
    public class ScreenManager
    {
        Dictionary<string, Screen> screens = new Dictionary<string, Screen>();
        string curScreen;

        public ScreenManager()
        {

        }
        public void Update()
        {
            screens[curScreen].Update();
        }
        public void Draw()
        {
            screens[curScreen].Draw();
        }
        public void setScreen(string newScreen)
        {
            //TODO: check to see if screen exists in screens<>
            curScreen = newScreen;
        }
        public void createScreen(string name, Screen newScreen)
        {
            screens.Add(name, newScreen);
        }
        public Screen getScreen()
        {
            return screens[curScreen];
        }
        public Screen getScreen(string scrn)
        {
            return screens[scrn];
        }
        public string getScreenName()
        {
            return curScreen;
        }
    }
}
