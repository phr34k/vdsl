using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using System.Diagnostics;

namespace Aurora.SandboxDesigner
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class Window1 : Window
    {
        public Window1()
        {
            InitializeComponent();
            canvas1.Header = "Untitled document";
            this.CommandBindings.Add( new CommandBinding( ApplicationCommands.Open,
                OpenCmdExecuted, OpenCmdCanExecute));
            this.CommandBindings.Add(new CommandBinding(ApplicationCommands.Save,
                SaveCmdExecuted, SaveCmdCanExecute));
            this.CommandBindings.Add(new CommandBinding(ApplicationCommands.New,
                NewCmdExecuted, NewCmdCanExecute));
            this.CommandBindings.Add(new CommandBinding(DebugCommands.Start,
                StartCmdExecuted, StartCmdCanExecute));
        }

        void NewCmdExecuted(object target, ExecutedRoutedEventArgs e)
        {
            try
            {
                canvas1.Header = "Untitled document";
                canvas1.Clear();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Unexpected error occured");
            }
        }

        void NewCmdCanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        void OpenCmdExecuted(object target, ExecutedRoutedEventArgs e)
        {
            try
            {
                Microsoft.Win32.OpenFileDialog dialog = new Microsoft.Win32.OpenFileDialog();
                dialog.Filter = "*.xml|*.xml";
                if (dialog.ShowDialog() == true) 
                {
                    try
                    {
                        using (FileStream s = File.OpenRead(dialog.FileName))
                        {
                            canvas1.Header = dialog.FileName;
                            canvas1.Clear();
                            canvas1.Load(s);
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Unexpected error occured");
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Unexpected error occured");
            }
        }

        void OpenCmdCanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        void SaveCmdExecuted(object target, ExecutedRoutedEventArgs e)
        {
            try
            {
                if (File.Exists(canvas1.Header.ToString()))
                {
                    try
                    {
                        using (FileStream s = File.OpenWrite(canvas1.Header.ToString()))
                        {
                            canvas1.Save(s);
                            s.SetLength(s.Position);
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Unexpected error occured");
                    }
                }
                else
                {
                    SaveAsCmdExecuted(target, e);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Unexpected error occured");
            }
        }

        void SaveCmdCanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        void SaveAsCmdExecuted(object target, ExecutedRoutedEventArgs e)
        {
            try
            {
                Microsoft.Win32.SaveFileDialog dialog = new Microsoft.Win32.SaveFileDialog();
                dialog.Filter = "*.xml|*.xml";
                if (dialog.ShowDialog() == true)
                {
                    try
                    {
                        using (FileStream s = File.OpenWrite(dialog.FileName))
                        {
                            canvas1.Header = dialog.FileName;
                            canvas1.Save(s);
                            s.SetLength(s.Position);
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Unexpected error occured");
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Unexpected error occured");
            }
        }

        void SaveAsCmdCanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        void StartCmdExecuted(object target, ExecutedRoutedEventArgs e)
        {
            try
            {
                string filename = string.Empty;
                if (canvas1.Header == null || !File.Exists(canvas1.Header.ToString()))
                {
                    filename = System.IO.Path.GetTempFileName();
                    using (FileStream s = File.OpenWrite(filename))
                    {
                        canvas1.Save(s);
                        s.SetLength(s.Position);
                    }
                }
                else
                {
                    filename = canvas1.Header.ToString();
                }


                Process p = new Process();
                p.StartInfo = new ProcessStartInfo();
                p.StartInfo.FileName = System.IO.Path.Combine(System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location), Environment.ExpandEnvironmentVariables(Properties.Settings.Default.vmexe) );

                StringBuilder builder = new StringBuilder();
                builder.Append("\"");
                builder.Append(System.IO.Path.Combine(System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location), Environment.ExpandEnvironmentVariables(Properties.Settings.Default.vmdb)));
                builder.Append("\"");
                builder.Append("  ");
                builder.Append("\"");
                builder.Append(filename);
                builder.Append("\"");

                p.StartInfo.Arguments = builder.ToString();
                p.Start();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Unexpected error occured");
            }
        }

        void StartCmdCanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            try
            {
                /*
                if (canvas1.Header == null)
                    return;
                if (!File.Exists(canvas1.Header.ToString()))
                    return;
                */
                foreach (object c in canvas1.Items)
                {
                    Controls.GraphNodeItem item = c as Controls.GraphNodeItem;
                    if (item != null && item.Typename == "Signal" && item.Name == "Execute")
                    {
                        e.CanExecute = true;
                    }
                }
            }
            catch (Exception ex)
            {
            }
        }

    }



    public class DebugCommands
    {
        private static RoutedCommand StartCommand = new RoutedCommand("Start",
            typeof(DebugCommands), new InputGestureCollection() {
                new KeyGesture(Key.F5, ModifierKeys.None) });

        public static RoutedCommand Start
        {
            get { return DebugCommands.StartCommand; }
        }
    }
}
