using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Aurora.SandboxDesigner.Internals
{
    class PathHelper
    {

        public static string PathFromProcess(string path)
        {
            return Path.Combine(Path.GetDirectoryName(System.Reflection.Assembly.GetEntryAssembly().Location), path);
        }

        public static string PathRelativeToProcess(string path)
        {
            return path.Replace(Path.GetDirectoryName(System.Reflection.Assembly.GetEntryAssembly().Location), "");
        }
    }
}
