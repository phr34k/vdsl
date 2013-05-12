using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Media;
using System.Xml;
using System.Windows.Data;
using System.IO;
using System.Windows.Input;

namespace Aurora.SandboxDesigner.Controls
{
    public interface ICloneable<T> : ICloneable
        where T : ICloneable<T>
    {
        new T Clone();
    }

    static class Extensions
    {
        public static List<T> Clone<T>(this List<T> listToClone) where T : ICloneable<T>
        {
            return listToClone.Select(item => (T)item.Clone()).ToList();
        }
    }

    [Serializable()]
    public enum GraphSocketSlot
    {
        Bidirectional,
        In,
        Out
    }

    [Serializable()]
    public class GraphSocket : ICloneable<GraphSocket>
    {
        #region Private Members

        private string name;
        private GraphSocketSlot slot;

        #endregion

        #region Public Members

        public GraphSocketSlot Slot
        {
            get { return slot; }
            set { slot = value; }
        }
        
        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        public string Type;

        #endregion

        #region ICloneable<GraphSocket> Members

        public GraphSocket Clone()
        {
            return new GraphSocket(
                Name, Slot
            );
        }

        #endregion

        #region ICloneable Members

        object ICloneable.Clone()
        {
            return Clone();
        }

        #endregion

        #region ctor / dtor

        public GraphSocket()
        {
        }

        public GraphSocket(string Name, GraphSocketSlot Slot)
        {
            this.name = Name;
            this.Type = string.Empty;
            this.slot = Slot;
        }

        #endregion
    }

    [Serializable()]
    public class GraphProperty : ICloneable<GraphProperty>
    {
        #region Private Members

        private List<GraphIType> preset = null;
        private string value = string.Empty;
        public object type = null;

        #endregion

        #region Public Members

        public GraphNodeItem Owner { get; set; }
        public string Name { get; set; }
        public string Value {
            get
            {
                return value;
            }
            set
            {
                if (this.value != value)
                {
                    this.value = value;
                    Owner.RaisePropertyChanged(Name);
                }
            }
        }
        public object Type { get { return type; } 
            set { 
                if (type != value) 
                {    
                    type = value; 
                    change(); 
                } } }
        public List<GraphIType> Types { get { return preset; } set { preset = value; } }

        #endregion        

        public event EventHandler OnChange;

        public void change()
        {
            EventHandler hs = OnChange;
            OnChange = null;
            if (hs != null)
                hs.Invoke(this, EventArgs.Empty);            
        }

        #region ICloneable<GraphProperty> Members

        public GraphProperty Clone()
        {
            var x = new GraphProperty(Owner, Name, Value, Type);
            x.preset = preset;
            return x;
        }

        #endregion

        #region ICloneable Members

        object ICloneable.Clone()
        {
            return Clone();
        }

        #endregion

        #region Ctor / Dtor

        public GraphProperty(GraphNodeItem Owner, string Name, string Value, object Type)
        {
            this.Owner = Owner;
            this.Name = Name;
            this.Value = Value;
            this.type = Type;
        }

        #endregion
    }

    [Serializable()]
    public class GraphNodeItem : System.ComponentModel.INotifyPropertyChanged, System.Runtime.Serialization.IDeserializationCallback
    {
        #region Private Members

        private static Random serialRandomizer = new Random();
        private static uint serialBits = 0;
        private string name;
        private string typename;
        private string category;
        private string shape;
        [NonSerialized()]
        private SolidColorBrush color = Brushes.Gold;
        private float a, r, g, b;

        #endregion        

        #region Public Members

        public uint NodeId;
        public List<GraphSocket> Socket = new List<GraphSocket>();
        public List<GraphProperty> properties = new List<GraphProperty>();

        public string Name
        {
            get { return name; }
            set
            {
                if (name != value)
                {
                    name = value;
                    RaisePropertyChanged("Name");
                }
            }
        }

        public string Typename
        {
            get { return typename; }
            set { typename = value; }
        }

        public string Category
        {
            get { return category; }
            set { category = value; }
        }

        public string Shape
        {
            get { return shape; }
            set { shape = value; }
        }

        public SolidColorBrush Brush
        {
            get { return color; }
            set { color = value; a = value.Color.ScA; r = value.Color.ScR; g = value.Color.ScG; b = value.Color.ScB; }
        }
        

        public IEnumerable<GraphSocket> Inputs
        {
            get
            {
                foreach (GraphSocket s in Socket)
                    if (s.Slot == GraphSocketSlot.In)
                        yield return s;
            }
        }

        public IEnumerable<GraphSocket> Outputs
        {
            get
            {
                foreach (GraphSocket s in Socket)
                    if( s.Slot == GraphSocketSlot.Out )
                        yield return s;
            }
        }

        public IEnumerable<GraphProperty> Properties
        {
            get
            {
                return properties;
            }
        }

        #endregion

        #region Ctor / dtor

        public GraphNodeItem()
        {
            uint random   = (uint)serialRandomizer.Next();
            uint variying = (random & 0x007FFFFFU) | ((++serialBits) & ((1 << 7) - 1)) << 23;
            uint hash     = random & 0xFF800000U;           
            for (int i = 0; i < 32; i++) {
                hash ^= (hash << 3) | (hash >> (32 - 3));
            }

            variying ^= hash;
            NodeId = variying;
        }

        public GraphNodeItem(uint NodeId)
        {
            this.NodeId = NodeId;
        }

        #endregion

        public void RaisePropertyChanged(string Name)
        {
            if (PropertyChanged != null)
                PropertyChanged.Invoke(this, new System.ComponentModel.PropertyChangedEventArgs(Name));
        }

        #region INotifyPropertyChanged Members
        
        [field: NonSerialized]
        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;

        #endregion

        #region IDeserializationCallback Members

        void System.Runtime.Serialization.IDeserializationCallback.OnDeserialization(object sender)
        {
            color = new SolidColorBrush(Color.FromScRgb(a,r,g,b));

            bool equals = Name == NodeId.ToString("x");

            uint random = (uint)serialRandomizer.Next();
            uint variying = (random & 0x007FFFFFU) | ((++serialBits) & ((1 << 7) - 1)) << 23;
            uint hash = random & 0xFF800000U;
            for (int i = 0; i < 32; i++)
            {
                hash ^= (hash << 3) | (hash >> (32 - 3));
            }

            variying ^= hash;
            NodeId = variying;
            if (equals)
            {
                Name = NodeId.ToString("x");
            }

        }

        #endregion
    }

    public interface GraphIType
    {
        string Name { get; set; }
        string Editor { get; set; }
    };
    
    [Serializable()]
    public class GraphType : GraphIType
    {
        private string name;
        private string editor;

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        public string Editor
        {
            get { return editor; }
            set { editor = value; }
        }
    };

    [Serializable()]
    public class GraphEnumType : Dictionary<string, uint>, GraphIType
    {
        private string name;
        private string editor;

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        public string Editor
        {
            get { return editor; }
            set { editor = value; }
        }
    }

    public class GraphViewItem
    {
        System.Windows.Data.CollectionViewSource view = new CollectionViewSource();

        public string Name { get; set; }
        public bool IsExpanded { get; set; }
        public System.Windows.Data.CollectionViewSource Items { get { return view; } }
        public object ItemsSource { get { return view.Source; } set { view.Source = value; } }
    };


    public class DesignerContextMenu : ContextMenu
    {
        private Point InsertPosition;

        public Point Position
        {
            get { return InsertPosition; }
            set { InsertPosition = value; }
        }        
    };


    public class GraphNodeDiagramDesignerCommands
    {
        private static RoutedCommand SelectTypeCommand = new RoutedCommand("SelectType",
            typeof(DesignerCanvas));

        public static RoutedCommand SelectType
        {
            get { return GraphNodeDiagramDesignerCommands.SelectTypeCommand; }
        }
    }

    public class GraphNodeDiagramDesigner : DesignerCanvas
    {
        public Database DesignerDatabase = Database.Empty;
        public static RoutedCommand ItemPressedCommand = new RoutedCommand("ItemPressed", 
            typeof(GraphNodeDiagramDesigner));

        private struct ConnectionPair
        {
            public GraphSocketSlot Slot { get; set; }
            public string Name { get; set; }
        }
       
        public struct Database
        {
            public Dictionary<string, GraphNodeItem> Templates;
            public Dictionary<string, object> Types;

            public static Database Empty
            {                
                get
                {
                    Database b = new Database();
                    b.Templates = new Dictionary<string, GraphNodeItem>();
                    b.Types = new Dictionary<string, object>();
                    return b;
                }                
            }
        }
        

        public Connector FromNode(object Item, object Name, GraphSocketSlot Slot)
        {
            try
            {
                //Find the designer item associated with the node
                DesignerItem item = (ItemContainerGenerator.ContainerFromItem(Item) as DesignerItem);
                ControlTemplate template = item.Template;

                //Apply the template forces it to load if it wasn't loaded
                item.ApplyTemplate();

                //Find the content presenter...
                ContentPresenter presenter = item.Template.FindName("Part_Content", item) as ContentPresenter;
                System.Reflection.PropertyInfo info = presenter != null ? presenter.GetType().GetProperty("Template", System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.GetProperty) : null;
                if (info != null)
                {
                    DataTemplate template2 = info.GetValue(presenter, null) as DataTemplate;

                    {
                        ItemsControl c = template2.FindName("Part_Inputs", presenter) as ItemsControl;
                        ItemsControl d = template2.FindName("Part_Outputs", presenter) as ItemsControl;
                        ItemsControl e = template2.FindName("Part_Parameters", presenter) as ItemsControl;

                        //var A12 = c.ItemContainerGenerator.Status;

                        {
                            var a = c != null ? c.ItemContainerGenerator.ContainerFromItem(Name) : null;
                            var b = d != null ? d.ItemContainerGenerator.ContainerFromItem(Name) : null;
                            var z = e != null ? e.ItemContainerGenerator.ContainerFromItem(Name) : null;

                            if (a != null) (a as ContentPresenter).ApplyTemplate();
                            if (b != null) (b as ContentPresenter).ApplyTemplate();
                            if (z != null) (z as ContentPresenter).ApplyTemplate();

                            object connector = null;
                            if (a != null)
                            {
                                if ((a as ContentPresenter).ContentTemplate != null)
                                {
                                    connector = (a as ContentPresenter).ContentTemplate.FindName("Part_Source", a as ContentPresenter);
                                    if (connector != null)
                                        return connector as Connector;
                                }
                            }

                            if (b != null)
                            {
                                if ((b as ContentPresenter).ContentTemplate != null)
                                {
                                    connector = (b as ContentPresenter).ContentTemplate.FindName("Part_Out", b as ContentPresenter);
                                    if (connector != null)
                                        return connector as Connector;
                                }
                            }

                            if (z != null)
                            {
                                if (Slot == GraphSocketSlot.In)
                                {
                                    if ((z as ContentPresenter).ContentTemplate != null)
                                    {
                                        connector = (z as ContentPresenter).ContentTemplate.FindName("Part_Source", z as ContentPresenter);
                                        if (connector != null)
                                            return connector as Connector;
                                    }
                                }
                                else if (Slot == GraphSocketSlot.Out)
                                {
                                    if ((z as ContentPresenter).ContentTemplate != null)
                                    {
                                        connector = (z as ContentPresenter).ApplyTemplate();
                                        connector = (z as ContentPresenter).ContentTemplate.FindName("Part_Out", z as ContentPresenter);
                                        if (connector != null)
                                            return connector as Connector;
                                    }
                                }
                            }


                            Connector retval = null;
                            FindConnector(item, x => {
                                return x.DataContext.GetType() == typeof(GraphSocket) &&
                                    x.DataContext == Name;                                
                            }, out retval);

                            /*                                   
                            object connector = a == null ? (b as ContentPresenter).ContentTemplate.FindName("Part_Source", b as ContentPresenter) : (a as ContentPresenter).ContentTemplate.FindName("Part_Source", a as ContentPresenter);
                            return connector as Connector;
                            */

                            return null;
                        }
                    }
                }
                else
                {
                    Connector retval = null;
                    FindConnector(item, x =>
                    {
                        return x.DataContext.GetType() == typeof(GraphSocket) &&
                            x.DataContext == Name;
                    }, out retval);
                    return retval;
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
           
            return null;
        }



        private static bool FindConnector(DependencyObject d, Predicate<Connector> predicate, out Connector element )
        {
            element = null;
            int max = VisualTreeHelper.GetChildrenCount(d);
            for (int i = 0; i < max; i++)
            {
                DependencyObject dc = VisualTreeHelper.GetChild(d, i);
                Connector connector = dc as Connector;

                if (connector != null)
                {
                    bool retval = predicate(connector);
                    element = connector;
                    if (retval) return true;                        
                }               
                else
                {
                    bool retval = FindConnector(dc, predicate, out element);
                    if (retval) return true;
                }
            }

            return false;
        }


       


        private void Reload()
        {
            Database database = LoadDesigner();

            GraphViewItem items = new GraphViewItem();
            List<string> types = new List<string>();
            types.Add("Comment");
            types.AddRange(database.Templates.Select(x => x.Key));
            types.Sort();

            items.Items.Source = database.Templates.GroupBy(x => x.Value.Category).Select(
                x => new GraphViewItem()
                {
                    Name = x.Key.ToString(),
                    ItemsSource = database.Templates.Where(z => z.Value.Category == x.Key.ToString()).Select(z =>
                        new GraphViewItem()
                        {
                            Name = z.Key
                        }
                    )
                }
            );



            this.ItemContainerStyleSelector = new GraphNodeItemContainerStyleSelector();
            this.ContextMenu = new DesignerContextMenu();
            this.ContextMenu.ItemsSource = items.Items.View;
            this.ContextMenu.Opened += new RoutedEventHandler(ContextMenu_Opened);
            DesignerDatabase = database;
        }

        void GraphNodeDiagramDesigner_ConnectionCanLink(object source, LinkCanEstablishEventArgs args)
        {
            args.IsAllowed = args.SourceConnector.DataContext.GetType() == args.TargetConnector.DataContext.GetType() && args.SourceConnector.Group != args.TargetConnector.Group;

            if (args.SourceConnector.DataContext is GraphProperty && args.TargetConnector.DataContext is GraphProperty)
            {
                if( args.IsAllowed )
                {
                    args.IsAllowed = (args.SourceConnector.DataContext as GraphProperty).Type == (args.TargetConnector.DataContext as GraphProperty).Type;
                }
            }

        }        

        void b_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            DesignerContextMenu menu = sender as DesignerContextMenu;
            if ( menu != null )
            {
                if ((e.OriginalSource as TreeViewItem) != null && (e.OriginalSource as TreeViewItem).DataContext == e.Parameter)
                {
                    GraphViewItem v = e.Parameter as GraphViewItem;
                    e.Handled = true;



                    //MenuItem source2 = e.OriginalSource as MenuItem;
                    //string header = this.ContextMenu.ItemContainerGenerator.ItemFromContainer(source2) as string;

                    string header = v.Name;
                    if (header == "Comment")
                    {
                        DesignerComment comment = new DesignerComment();
                        //ConnectionLayer.Children.Add(comment);

                        Items.Add(comment);
                        comment.Arrange(new Rect(0, 0, 0, 0));
                        ResizeableCanvas.SetLeft(comment, menu.Position.X - (comment.DesiredSize.Width / 2));
                        ResizeableCanvas.SetTop(comment, menu.Position.Y - (comment.DesiredSize.Height / 2));

                    }
                    else
                    {
                        while (true)
                        {
                            GraphNodeItem item = new GraphNodeItem();
                            GraphNodeItem contents = Items.OfType<GraphNodeItem>().Where<GraphNodeItem>(x =>
                                x.NodeId == item.NodeId).SingleOrDefault();
                            if (contents == null)
                            {
                                GraphNodeItem templateItem = null;
                                if (DesignerDatabase.Templates.TryGetValue(header, out templateItem))
                                {
                                    item.Name = item.NodeId.ToString("x");
                                    item.Typename = header;
                                    item.Shape = templateItem.Shape;
                                    item.Brush = templateItem.Brush;
                                    item.properties = templateItem.properties.Clone();
                                    for (int i = 0; i < item.properties.Count; i++)
                                        item.properties[i].Owner = item;
                                    item.Socket = templateItem.Socket.Clone();


                                    canvasLeftLookup[item] = menu.Position.X /*+ HorizontalScrollOffset*/;
                                    canvasTopLookup[item] = menu.Position.Y /*+ VerticalScrollOffset*/;
                                    Items.Add(item);
                                }

                                break;
                            }
                        }
                    }
                }
            }            
        }
      




        protected override void contentitem_Loaded(object sender, RoutedEventArgs e)
        {                               
        }



        

        void ContextMenu_Opened(object sender, RoutedEventArgs e)
        {
            if (CommandBindings.Count == 0)
            {
                CommandBinding b = new CommandBinding();
                b.Command = ItemPressedCommand;
                b.Executed += new ExecutedRoutedEventHandler(b_Executed);
                (sender as ContextMenu).CommandBindings.Add(b);
            }

            ItemsPresenter p = GetTemplateChild("Items") as ItemsPresenter;
            (sender as DesignerContextMenu).Position = Mouse.GetPosition(p);           
            TextBox Part_UINodeFilter = ContextMenu.Template.FindName("Part_UINodeFilter", ContextMenu) as TextBox;
            TreeView Part_TreeView = ContextMenu.Template.FindName("Part_TreeView", ContextMenu) as TreeView;

            Part_UINodeFilter.TextChanged += delegate(object sender2, TextChangedEventArgs e2)
            {
                ContextMenu menu = sender as ContextMenu;
                if (menu == null) return;
                System.ComponentModel.ICollectionView v = menu.ItemsSource as System.ComponentModel.ICollectionView;                

                var FilterPredicate = new Predicate<object>(x => 
                {                    
                    var a = x as GraphViewItem;
                    if (a.Items.View == null)
                    {
                        return a.Name.ToLower().Contains((sender2 as TextBox).Text.ToLower()); 
                    }

                    using (a.Items.View.DeferRefresh())
                    {
                        a.Items.View.Filter = v.Filter;
                    }

                    if (a.Items.View.IsEmpty)
                    {
                        return a.Name.ToLower().Contains((sender2 as TextBox).Text.ToLower()); 
                    }
                    else
                    {
                        if (string.IsNullOrEmpty((sender2 as TextBox).Text))
                            return true;
                        return !a.Items.View.IsEmpty;
                    }                    
                });

                using (v.DeferRefresh())
                {
                    v.Filter = FilterPredicate;
                }
            };

            Part_UINodeFilter.PreviewKeyDown += delegate(object sender2, KeyEventArgs e2)
            {
                Dispatcher.BeginInvoke( (Action)delegate()
                {                
                    try
                    {
                        if (e2.Key == Key.Down)
                        {
                            UIElement senderElement = sender2 as UIElement;
                            UIElement focusedElement = FocusManager.GetFocusedElement(senderElement) as UIElement;
                            bool result = focusedElement.MoveFocus(new TraversalRequest(FocusNavigationDirection.Next));
                        }
                        else if (e2.Key == Key.Up)
                        {
                            UIElement senderElement = sender2 as UIElement;
                            UIElement focusedElement = FocusManager.GetFocusedElement(senderElement) as UIElement;
                            bool result = focusedElement.MoveFocus(new TraversalRequest(FocusNavigationDirection.Previous));
                        }
                    }
                    catch (Exception ex)
                    {
                    }
                });
            };
        }


        private Database LoadDesigner()
        {
            Database designerDatabase = Database.Empty;

            try
            {                
                string path = /**/
                    System.IO.Path.Combine(System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location), /*"flow.xml"*/
                    Environment.ExpandEnvironmentVariables(Aurora.SandboxDesigner.Properties.Settings.Default.vmdb));
                using (FileStream stream = File.Open(path, FileMode.Open))
                using (System.Xml.XmlTextReader reader = new System.Xml.XmlTextReader(stream))
                {
                    GraphEnumType enumType = null;
                    GraphNodeItem item = null;
                    List<KeyValuePair<KeyValuePair<uint, string>, KeyValuePair<uint, string>>> connectionPair =
                        new List<KeyValuePair<KeyValuePair<uint, string>, KeyValuePair<uint, string>>>();
                    Dictionary<uint, GraphNodeItem> nodeResolver = new Dictionary<uint, GraphNodeItem>();
                    while (reader.Read())
                    {
                        if (reader.NodeType == XmlNodeType.XmlDeclaration)
                        {
                        }                       
                        else if (reader.NodeType == XmlNodeType.Element && reader.Name == "Type")
                        {
                            object retval = null;
                            string id = reader.GetAttribute("Name");
                            if (designerDatabase.Types.TryGetValue(id, out retval))
                            {
                                System.Diagnostics.Debugger.Break();
                            }
                            else
                            {
                                GraphType type = new GraphType();
                                type.Name = id;
                                type.Editor = reader.GetAttribute("Editor");
                                designerDatabase.Types[id] = type;
                            }
                        }
                        else if (reader.NodeType == XmlNodeType.Element && reader.Name == "Enum")
                        {
                            object retval = null;
                            string id = reader.GetAttribute("Name");
                            if (designerDatabase.Types.TryGetValue(id, out retval))
                            {
                                System.Diagnostics.Debugger.Break();
                            }
                            else
                            {
                                enumType = new GraphEnumType();
                                enumType.Name = id;
                                enumType.Editor = reader.GetAttribute("Editor");
                                designerDatabase.Types[id] = enumType;
                            }
                        }
                        else if (reader.NodeType == XmlNodeType.Element && reader.Name == "Node")
                        {
                            string id = reader.GetAttribute("Name");
                            if (designerDatabase.Templates.TryGetValue(id, out item))
                            {
                                System.Diagnostics.Debugger.Break();
                            }
                            else
                            {
                                item = new GraphNodeItem();
                                designerDatabase.Templates[id] = item;
                            }


                            string color = reader.GetAttribute("Color");
                            if (color == null || color == "Gold")
                                item.Brush = Brushes.Gold;
                            else if (color == "Red")
                                item.Brush = Brushes.Red;
                            else if (color == "Blue")
                                item.Brush = Brushes.Blue;
                            else if (color == "Purple")
                                item.Brush = Brushes.Purple;
                            else if (color == "LimeGreen")
                                item.Brush = Brushes.LimeGreen;


                            string category = reader.GetAttribute("Category");
                            string shape = reader.GetAttribute("Shape");
                            item.Category = String.IsNullOrEmpty(category) ? "Default" : category;
                            item.Shape = String.IsNullOrEmpty(shape) ? "Default" : shape;                            
                        }
                        else if (reader.NodeType == XmlNodeType.EndElement && reader.Name == "Node")
                        {
                            item = null;
                        }
                        else if (reader.NodeType == XmlNodeType.EndElement && reader.Name == "Node")
                        {
                            enumType = null;
                        }
                        else if (reader.NodeType == XmlNodeType.Element && reader.Name == "Member")
                        {
                            if (item != null)
                            {
                                string name = reader.GetAttribute("Name");
                                string value = reader.GetAttribute("Default");
                                string type = reader.GetAttribute("Type");
                                string visible = reader.GetAttribute("Visible");
                                string[] types = (reader.GetAttribute("Types") == null ? string.Empty : reader.GetAttribute("Types")).Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                                


                                if (string.IsNullOrEmpty(visible) && visible != "Hidden")
                                {
                                    object typeInformation = null;
                                    if (!string.IsNullOrEmpty(type)) designerDatabase.Types.TryGetValue(type, out typeInformation);
                                    GraphProperty p = new GraphProperty(item, name, value, typeInformation);                                    
                                    if (types.Length > 0)
                                    {
                                        p.Types = new List<GraphIType>();
                                        foreach (string x in types)
                                            p.Types.Add(designerDatabase.Types[x] as GraphIType);
                                    }
                                    
                                    item.properties.Add(p);

                                    
                                }                                
                            }
                            else if (enumType != null)
                            {
                                string name = reader.GetAttribute("Name");
                                string type = reader.GetAttribute("Value");
                                if( type.StartsWith("0x") )
                                    enumType[name] = uint.Parse(type.Replace("0x",""), System.Globalization.NumberStyles.HexNumber);
                                else
                                    enumType[name] = uint.Parse(type);
                            }
                        }
                        else if (reader.NodeType == XmlNodeType.Element && reader.Name == "Signal")
                        {
                            if (item != null)
                            {
                                string name = reader.GetAttribute("Name");
                                string type = reader.GetAttribute("Type");

                                if (type == "In")
                                {
                                    item.Socket.Add(new GraphSocket()
                                    {
                                        Name = name,
                                        Slot = GraphSocketSlot.In
                                    });
                                }
                                else if (type == "Out")
                                {
                                    item.Socket.Add(new GraphSocket()
                                    {
                                        Name = name,
                                        Slot = GraphSocketSlot.Out
                                    });
                                }
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
            }

            return designerDatabase;
        }

        public void Clear()
        {
            _loading = true;
            canvasLeftLookup.Clear();
            canvasTopLookup.Clear();
            //ConnectionLayer.Children.Clear();
            MyConnections.Clear();
            Items.Clear();
            _loading = false;
        }

        public void Load(System.IO.Stream stream)
        {
            try
            {
                GraphNodeItem item = null;
                List<DesignerConnection> mconnections = new List<DesignerConnection>();

                _loading = true;
                BeginInit();
                using (System.Xml.XmlTextReader reader = new System.Xml.XmlTextReader(stream))
                {
                    List<KeyValuePair<KeyValuePair<uint, ConnectionPair>, KeyValuePair<uint, ConnectionPair>>> connectionPair =
                        new List<KeyValuePair<KeyValuePair<uint, ConnectionPair>, KeyValuePair<uint, ConnectionPair>>>();
                    Dictionary<uint, GraphNodeItem> nodeResolver = new Dictionary<uint, GraphNodeItem>();
                    while (reader.Read())
                    {
                        if (reader.NodeType == XmlNodeType.XmlDeclaration)
                        {
                        }
                        else if (reader.NodeType == XmlNodeType.Element && reader.Name == "Comment")
                        {
                            try
                            {
                                string name = reader.GetAttribute("Designer.Text");
                                DesignerComment comment = new DesignerComment();
                                RoutedEventHandler eh = null;
                                eh = (e, o) =>
                                {
                                    comment.Comment = name;
                                    comment.Loaded -= eh;
                                };

                                comment.Loaded += eh;                                                               
                                Items.Add(comment);

                                if (string.IsNullOrEmpty(reader.GetAttribute("Designer.OffsetX")) == false)
                                    ResizeableCanvas.SetLeft(comment, double.Parse(reader.GetAttribute("Designer.OffsetX"), System.Globalization.CultureInfo.InvariantCulture));
                                if (string.IsNullOrEmpty(reader.GetAttribute("Designer.OffsetY")) == false)
                                    ResizeableCanvas.SetTop(comment, double.Parse(reader.GetAttribute("Designer.OffsetY"), System.Globalization.CultureInfo.InvariantCulture));

                                reader.Skip();
                            }
                            catch (Exception ex)
                            {
                            }
                        }
                        else if (reader.NodeType == XmlNodeType.Element && reader.Name == "Node")
                        {
                            uint v;
                            string id = reader.GetAttribute("Id");
                            if (uint.TryParse(id, System.Globalization.NumberStyles.HexNumber, null, out v))
                            {
                                item = new GraphNodeItem(v) { Name = reader.GetAttribute("Designer.Name"), Typename = reader.GetAttribute("Type") };
                                GraphNodeItem template = null;

                                string type = reader.GetAttribute("Type");
                                if (DesignerDatabase.Templates.TryGetValue(type, out template))
                                {
                                    item.Category = template.Category;
                                    item.Brush = template.Brush;
                                    item.Shape = template.Shape;
                                    item.properties = template.properties.Clone();
                                    item.Socket = template.Socket.Clone();
                                    for (int i = 0; i < item.properties.Count; i++)
                                    {
                                        item.properties[i].Owner = item;
                                        string value = reader.GetAttribute(item.properties[i].Name);
                                        if (value != null)
                                            item.properties[i].Value = value;
                                    }

                                }
                                else
                                {
                                    MessageBox.Show("Error\r\n");
                                }

                                nodeResolver[item.NodeId] = item;
                                
                                if (string.IsNullOrEmpty(reader.GetAttribute("Designer.OffsetX")) == false)
                                    canvasLeftLookup[item] = double.Parse(reader.GetAttribute("Designer.OffsetX"), System.Globalization.CultureInfo.InvariantCulture);
                                if (string.IsNullOrEmpty(reader.GetAttribute("Designer.OffsetY")) == false)
                                    canvasTopLookup[item] = double.Parse(reader.GetAttribute("Designer.OffsetY"), System.Globalization.CultureInfo.InvariantCulture);
                            }

                            //reader.Skip();
                            if (reader.IsEmptyElement)
                            {
                                Items.Add(item);
                                item = null;
                            }
                        }
                        else if (reader.NodeType == XmlNodeType.Element && reader.Name == "Property")
                        {
                            GraphProperty property = item.properties.Where(x => x.Name == reader.GetAttribute("Name")).FirstOrDefault();
                            if (property != null && property.Types != null)
                            {
                                string name = reader.GetAttribute("Type");
                                var type = property.Types.Where(x => x.Name == name).FirstOrDefault();
                                if (type != null) property.Type = type;                                
                            }                            
                        }
                        else if (reader.NodeType == XmlNodeType.EndElement && reader.Name == "Node")
                        {                            
                            Items.Add(item);
                            item = null;
                        }
                        else if (reader.NodeType == XmlNodeType.Element && reader.Name == "Connection")
                        {
                            string[] source = reader.GetAttribute("Source").Split(new char[] { '.' }, 2, StringSplitOptions.RemoveEmptyEntries);
                            string[] target = reader.GetAttribute("Target").Split(new char[] { '.' }, 2, StringSplitOptions.RemoveEmptyEntries);
                            uint sourceComponent = uint.Parse(source[0], System.Globalization.NumberStyles.HexNumber);
                            uint targetComponent = uint.Parse(target[0], System.Globalization.NumberStyles.HexNumber);
                            GraphSocketSlot sourceSlot = GraphSocketSlot.In;
                            GraphSocketSlot targetSlot = GraphSocketSlot.Out;

                            switch (reader.GetAttribute("SourceType"))
                            {
                                case "In":
                                    sourceSlot = GraphSocketSlot.In;
                                    break;
                                case "Out":
                                    sourceSlot = GraphSocketSlot.Out;
                                    break;
                                case null:
                                    sourceSlot = GraphSocketSlot.In;
                                    break;
                            }

                            switch (reader.GetAttribute("TargetType"))
                            {
                                case "In":
                                    targetSlot = GraphSocketSlot.In;
                                    break;
                                case "Out":
                                    targetSlot = GraphSocketSlot.Out;
                                    break;
                                case null:
                                    targetSlot = GraphSocketSlot.Out;
                                    break;
                            }


                            connectionPair.Add(
                                new KeyValuePair<KeyValuePair<uint, ConnectionPair>, KeyValuePair<uint, ConnectionPair>>(
                                    new KeyValuePair<uint, ConnectionPair>(sourceComponent, new ConnectionPair() { Name = source[1], Slot = sourceSlot }),
                                    new KeyValuePair<uint, ConnectionPair>(targetComponent, new ConnectionPair() { Name = target[1], Slot = targetSlot })
                                )
                            );
                        }
                    }

                    EndInit();

                    //Force the dispatcher to finish generating all items after we loaded them to ensure the state after this occurs problem-less
                    //without complex many-wait-for-all scenario's. Essentionally we reroute the dispatcher logic untill at a point we say it's 
                    //okay to continue processing the logic, this point of time should be when the ItemContainerGenerator finishes generating the 
                    //items.
                    InvalidateVisual();
                    System.Windows.Threading.DispatcherFrame frame = new System.Windows.Threading.DispatcherFrame(false);
                    EventHandler handler = delegate(object obj, EventArgs args) { frame.Continue = false; };
                    this.LayoutUpdated += handler;
                    System.Windows.Threading.Dispatcher.PushFrame(frame);
                    this.LayoutUpdated -= handler;





                    foreach (KeyValuePair<KeyValuePair<uint, ConnectionPair>, KeyValuePair<uint, ConnectionPair>> pairs in connectionPair)
                    {
                        GraphNodeItem souceItem = null, targetItem = null;
                        if (nodeResolver.TryGetValue(pairs.Key.Key, out souceItem) && nodeResolver.TryGetValue(pairs.Value.Key, out targetItem))
                        {
                            Connector sourceConnector = null, targetConnector = null;
                            for (int i = 0; i < souceItem.Socket.Count; i++)
                                if (souceItem.Socket[i].Name == pairs.Key.Value.Name)
                                    sourceConnector = FromNode(souceItem, souceItem.Socket[i], pairs.Key.Value.Slot);

                            for (int i = 0; i < targetItem.Socket.Count; i++)
                                if (targetItem.Socket[i].Name == pairs.Value.Value.Name)
                                    targetConnector = FromNode(targetItem, targetItem.Socket[i], pairs.Value.Value.Slot);

                            for (int i = 0; i < souceItem.properties.Count; i++)
                                if (souceItem.properties[i].Name == pairs.Key.Value.Name)
                                    sourceConnector = FromNode(souceItem, souceItem.properties[i], pairs.Key.Value.Slot);

                            for (int i = 0; i < targetItem.properties.Count; i++)
                                if (targetItem.properties[i].Name == pairs.Value.Value.Name)
                                    targetConnector = FromNode(targetItem, targetItem.properties[i], pairs.Value.Value.Slot);

                            DesignerConnection dcon = new DesignerConnection()
                            {
                                Source = sourceConnector,
                                Sink = targetConnector
                            };

                            mconnections.Add(dcon);
                            //ConnectionLayer.Children.Add(dcon);
                        }
                        else
                        {
                            Console.WriteLine("Connection not found");
                        }
                    }
                }

                //Change();


                MyConnections.AddRange(mconnections);
            }
            catch (Exception ex)
            {
                throw ex;
            }
            finally
            {
                EndInit();
                _loading = false;            
            }                       
        }

        public void Save(System.IO.Stream stream)
        {
            XmlWriterSettings settings = new XmlWriterSettings();
            settings.Encoding = Encoding.GetEncoding("Windows-1252");
            settings.Indent = true;
            settings.NewLineOnAttributes = true;
            settings.NewLineChars = Environment.NewLine;

            using (System.Xml.XmlWriter writer = System.Xml.XmlTextWriter.Create(stream, settings))
            {
                
                writer.WriteStartDocument();
                writer.WriteStartElement("Flow");


                foreach (DesignerComment item in Items.OfType<DesignerComment>())
                {
                    DesignerComment designerItem = ItemContainerGenerator.ContainerFromItem(item) as DesignerComment;
                    writer.WriteStartElement("Comment");
                    writer.WriteAttributeString("Designer.Text", string.IsNullOrEmpty(item.Comment) ? "" : item.Comment);
                    writer.WriteAttributeString("Designer.OffsetX", ResizeableCanvas.GetLeft(designerItem).ToString(System.Globalization.CultureInfo.InvariantCulture));
                    writer.WriteAttributeString("Designer.OffsetY", ResizeableCanvas.GetTop(designerItem).ToString(System.Globalization.CultureInfo.InvariantCulture));
                    writer.WriteEndElement();
                }

                foreach (GraphNodeItem item in Items.OfType<GraphNodeItem>())
                {
                    DesignerItem designerItem = ItemContainerGenerator.ContainerFromItem(item) as DesignerItem;

                    writer.WriteStartElement("Node");
                    writer.WriteAttributeString("Id", item.NodeId.ToString("X"));
                    writer.WriteAttributeString("Type", item.Typename);
                    writer.WriteAttributeString("Designer.Name", string.IsNullOrEmpty(item.Name) ? "" : item.Name);

                    foreach (GraphProperty prop in item.properties)
                    {
                        if( string.IsNullOrEmpty(prop.Value) == false )
                            writer.WriteAttributeString(prop.Name, prop.Value);                        
                    }


                    writer.WriteAttributeString("Designer.OffsetX", ResizeableCanvas.GetLeft(designerItem).ToString(System.Globalization.CultureInfo.InvariantCulture));
                    writer.WriteAttributeString("Designer.OffsetY", ResizeableCanvas.GetTop(designerItem).ToString(System.Globalization.CultureInfo.InvariantCulture));

                    foreach (GraphProperty prop in item.properties)
                    {
                        if (prop.Type != null )
                        {
                            writer.WriteStartElement("Property");
                            writer.WriteAttributeString("Name", prop.Name );
                            writer.WriteAttributeString("Type", (prop.Type as GraphIType).Name);
                            writer.WriteEndElement();
                        }
                    }                    

                    writer.WriteEndElement();                                      
                }

                try
                {
                    foreach (DesignerConnection connection in Connections)
                    {
                        DesignerItem sourceDesignerItem = Internals.UIHelper.FindAnchestor<DesignerItem>(connection.Source);
                        DesignerItem targetDesignerItem = Internals.UIHelper.FindAnchestor<DesignerItem>(connection.Sink);
                        GraphNodeItem sourceItem = sourceDesignerItem != null ? ItemContainerGenerator.ItemFromContainer(sourceDesignerItem) as GraphNodeItem : null;
                        GraphNodeItem targetItem = targetDesignerItem != null ? ItemContainerGenerator.ItemFromContainer(targetDesignerItem) as GraphNodeItem : null;
                        if (sourceItem != null && targetItem != null)
                        {
                            writer.WriteStartElement("Connection");

                            if (connection.Source.DataContext.GetType() == typeof(GraphSocket) && connection.Source.DataContext.GetType() == typeof(GraphSocket))
                            {
                                if (sourceItem.Socket.Find((e) => e.Name == ((GraphSocket)connection.Source.DataContext).Name).Slot == GraphSocketSlot.In)
                                {
                                    string sname = sourceItem.Socket.Find((e) => e.Name == ((GraphSocket)connection.Source.DataContext).Name).Name;
                                    string tname = targetItem.Socket.Find((e) => e.Name == ((GraphSocket)connection.Sink.DataContext).Name).Name;
                                    writer.WriteAttributeString("Source", sourceItem.NodeId.ToString("X") + "." + sname);
                                    writer.WriteAttributeString("Target", targetItem.NodeId.ToString("X") + "." + tname);
                                    writer.WriteAttributeString("Type", "Element");
                                }
                                else
                                {
                                    string sname = sourceItem.Socket.Find((e) => e.Name == ((GraphSocket)connection.Source.DataContext).Name).Name;
                                    string tname = targetItem.Socket.Find((e) => e.Name == ((GraphSocket)connection.Sink.DataContext).Name).Name;
                                    writer.WriteAttributeString("Source", targetItem.NodeId.ToString("X") + "." + tname);
                                    writer.WriteAttributeString("Target", sourceItem.NodeId.ToString("X") + "." + sname);
                                    writer.WriteAttributeString("Type", "Element");
                                }
                            }
                            else if (connection.Source.DataContext.GetType() == typeof(GraphProperty) && connection.Source.DataContext.GetType() == typeof(GraphProperty))
                            {
                                if (connection.Source.Name == "Part_Out")
                                {
                                    string sname = sourceItem.properties.Find((e) => e.Name == ((GraphProperty)connection.Source.DataContext).Name).Name;
                                    string tname = targetItem.properties.Find((e) => e.Name == ((GraphProperty)connection.Sink.DataContext).Name).Name;
                                    string stype = connection.Source.Name == "Part_Out" ? "Out" : "In";
                                    string ttype = connection.Sink.Name == "Part_Out" ? "Out" : "In";

                                    writer.WriteAttributeString("Source", targetItem.NodeId.ToString("X") + "." + tname);
                                    writer.WriteAttributeString("Target", sourceItem.NodeId.ToString("X") + "." + sname);
                                    writer.WriteAttributeString("SourceType", ttype);
                                    writer.WriteAttributeString("TargetType", stype);
                                    writer.WriteAttributeString("Type", "Property");
                                }
                                else
                                {
                                    string sname = sourceItem.properties.Find((e) => e.Name == ((GraphProperty)connection.Source.DataContext).Name).Name;
                                    string tname = targetItem.properties.Find((e) => e.Name == ((GraphProperty)connection.Sink.DataContext).Name).Name;
                                    string stype = connection.Source.Name == "Part_Out" ? "Out" : "In";
                                    string ttype = connection.Sink.Name == "Part_Out" ? "Out" : "In";

                                    writer.WriteAttributeString("Source", sourceItem.NodeId.ToString("X") + "." + sname);
                                    writer.WriteAttributeString("Target", targetItem.NodeId.ToString("X") + "." + tname);
                                    writer.WriteAttributeString("SourceType", stype);
                                    writer.WriteAttributeString("TargetType", ttype);
                                    writer.WriteAttributeString("Type", "Property");
                                }
                            }

                            writer.WriteEndElement();

                        }
                    }
                }
                catch (Exception ex)
                {
                }

                writer.WriteEndElement();                
                writer.WriteEndDocument();
            }
        }


        protected override void Refresh()
        {
            using (MemoryStream ms = new MemoryStream())
            {
                //Save to memory stream
                Save(ms);
                Clear();

                //Refresh catalog
                Reload();
                
                //Reload file
                ms.Position = 0;
                Load(ms);
            }
        }


        // Helper to search up the VisualTree
        private static T FindAnchestor<T>(DependencyObject current)
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

        private class GraphNodeConverter : IValueConverter
        {
            static char[] split = new char[] { '.' };

            #region IValueConverter Members

            public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
            {
                return (value as string).Split(split, 2)[0];
            }

            public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
            {
                throw new NotImplementedException();
            }

            #endregion
        }

        public GraphNodeDiagramDesigner()
        {
            Reload();
            this.ConnectionCanLink += new LinkCanEstablishEventHandler(GraphNodeDiagramDesigner_ConnectionCanLink);            
        }

        static GraphNodeDiagramDesigner()
        {
            CommandManager.RegisterClassCommandBinding(typeof(GraphNodeDiagramDesigner), new CommandBinding(GraphNodeDiagramDesignerCommands.SelectType, CommandSelectTypeHandler));
        }

        private static void CommandSelectTypeHandler(object target, ExecutedRoutedEventArgs e)
        {
            GraphNodeDiagramDesigner canvas = (target as GraphNodeDiagramDesigner);
            if (e.Command == GraphNodeDiagramDesignerCommands.SelectType)
            {
                BooleanField f = e.OriginalSource as BooleanField;
                (f.DataContext as GraphProperty).change();                
            }
        }
     
    }

    

    public class GraphNodeItemContainerStyleSelector : StyleSelector
    {
        public override Style SelectStyle(object item, DependencyObject container)
        {
            /*
            object[] obj = item.GetType().GetCustomAttributes(typeof(GraphNodeCategoryAttribute), false);
            string category = obj.Length > 0 ? (obj[0] as GraphNodeCategoryAttribute).Name : "Default";

            switch (category)
            {
                case "Parameters":
                    return (FindAnchestor<DesignerCanvas>(container) as DesignerCanvas).Resources["ParameterGroupStyle"] as Style;
                case "Root":
                    return (FindAnchestor<DesignerCanvas>(container) as DesignerCanvas).Resources["RootGroupStyle"] as Style;
                default:
                    return (FindAnchestor<DesignerCanvas>(container) as DesignerCanvas).Resources["DefaultStyle"] as Style;
            }
             * */
            DesignerCanvas canvas = (FindAnchestor<DesignerCanvas>(container) as DesignerCanvas);
            GraphNodeItem node = item as GraphNodeItem;
            if (node != null && node.Shape != "Default") {
                var s = canvas.Resources[node.Shape] as Style;
                if (s != null)
                {
                    return s;
                }
            }

            return canvas.Resources[ typeof(GraphNodeItem)] as Style;
        }

        // Helper to search up the VisualTree
        private static T FindAnchestor<T>(DependencyObject current)
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
    }



    public class GraphNodeParameterTemplateSelector : DataTemplateSelector 
    {
        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            DesignerCanvas canvas = (FindAnchestor<DesignerCanvas>(container) as DesignerCanvas);
            GraphProperty property = item as GraphProperty;

            if (property != null)
            {
                property.OnChange += delegate(object x, EventArgs x2)
                {
                    GraphProperty propertyx = property;
                    var z = propertyx.Type;
                    ContentPresenter presenter = container as ContentPresenter;
                    object c = presenter.GetValue(ContentPresenter.ContentProperty);
                    //presenter.SetValue(ContentPresenter.ContentTemplateSelectorProperty, null);
                    //presenter.SetValue(ContentPresenter.ContentTemplateSelectorProperty, c);
                    
                    System.Windows.Threading.Dispatcher.CurrentDispatcher.BeginInvoke((Action)delegate()
                    {
                        System.Windows.Threading.Dispatcher.CurrentDispatcher.BeginInvoke((Action)delegate()
                        {
                            property.OnChange += delegate(object x3, EventArgs x4)
                            {
                                if (property.Type == null)
                                {
                                    presenter.SetValue(ContentPresenter.ContentProperty, c);
                                    propertyx.type = z;
                                }
                            };

                            presenter.SetValue(ContentPresenter.ContentProperty, null);
                        }, System.Windows.Threading.DispatcherPriority.Normal, new object[] { });                                              
                    }, System.Windows.Threading.DispatcherPriority.Normal, new object[] { });


                    




                    


                };
            }

            if (property != null && property.Type != null)
            {
                 if (property.Type is GraphIType && !String.IsNullOrEmpty((property.Type as GraphIType).Editor))
                {
                    var s = canvas.Resources[(property.Type as GraphIType).Editor]
                        as DataTemplate;
                    if (s != null)
                        return s;
                }
            }

            //return canvas.Resources["MyParam"] as DataTemplate;   
            //var v = (container as FrameworkElement).Resources[typeof(GraphProperty)];
            //return v as DataTemplate;


            return null;             
        }


        // Helper to search up the VisualTree
        private static T FindAnchestor<T>(DependencyObject current)
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
    }

    public class GraphEnumConverter : IValueConverter
    {
        #region IValueConverter Members

        static List<string> v = new List<string>();

        object IValueConverter.Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            GraphProperty p = value as GraphProperty;
            if (p != null && p.Type is GraphEnumType )
            {
                return p.Type;
            }
            else
            {
                return v;
            }
        }

        object IValueConverter.ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }

        #endregion
    }

    public class GraphPropertySelector : IValueConverter
    {

        #region IValueConverter Members

        object IValueConverter.Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {            
            IEnumerable<GraphSocket> v = value as IEnumerable<GraphSocket>;
            return v.Skip(Convert.ToInt32(parameter)).FirstOrDefault();
        }

        object IValueConverter.ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }

        #endregion
    };

    public class GraphTypesVisibillitySelector : IValueConverter
    {

        #region IValueConverter Members

        object IValueConverter.Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            if (value == null)
            {
                return Visibility.Collapsed;
            }
            else
            {
                return Visibility.Visible;
            }
        }

        object IValueConverter.ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }

        #endregion
    };




}

