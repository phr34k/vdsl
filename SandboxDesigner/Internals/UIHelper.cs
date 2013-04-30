using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Media;

namespace Aurora.SandboxDesigner.Internals
{
    public class UIHelper
    {
        // Helper to search up the VisualTree
        public static T FindAnchestor<T>(DependencyObject current)
            where T : DependencyObject
        {
            do
            {
                if (current is T)
                {
                    return (T)current;
                }
                current = VisualTreeHelper.GetParent(current);
            }
            while (current != null);
            return null;
        }

        public static T TryFindParent<T>(DependencyObject current)
            where T : DependencyObject
        {
            return FindAnchestor<T>(current);
        }
    }
}
