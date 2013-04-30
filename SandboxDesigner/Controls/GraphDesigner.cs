using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Controls.Primitives;
using System.Windows.Threading;
using System.Windows.Controls;
using System.ComponentModel;
using System.Windows.Media;
using System.Globalization;
using System.Windows.Data;
using Aurora.SandboxDesigner.Internals;
using Aurora.SandboxDesigner.Controls;
using System.Runtime.Serialization;

namespace Aurora.SandboxDesigner.Controls
{
    public class LinkEventArgs : EventArgs
    {
        public object Source { get; set; }
        public object Target { get; set; }
        public Connector SourceConnector { get; set; }
        public Connector TargetConnector { get; set; }
    }
    public class LinkCanEstablishEventArgs : LinkEventArgs
    {
        public bool IsAllowed { get; set; }
    }

    public delegate void LinkCanEstablishEventHandler(object source, LinkCanEstablishEventArgs args);
    public delegate void LinkChangedEventHandler(object source, LinkEventArgs args);
    public delegate void LinkPriorityChangedEventHandler(object source, object item1, object item2, float priority);
    public delegate void ItemDoubleClickedEventHandler(object source, object item);


    internal static class BooleanBoxes
    {
        internal static object FalseBox;
        internal static object TrueBox;

        static BooleanBoxes()
        {
            TrueBox = true;
            FalseBox = false;
        }

        internal static object Box(bool value)
        {
            if (value)
            {
                return TrueBox;
            }
            return FalseBox;
        }
    }



    public class DesignerScrollViewer : ScrollViewer
    {
        internal static readonly DependencyPropertyKey HasHeaderPropertyKey;
        public static readonly DependencyProperty HasHeaderProperty;        
        public static readonly DependencyProperty HeaderProperty;
        public static readonly DependencyProperty HeaderStringFormatProperty;
        public static readonly DependencyProperty HeaderTemplateProperty;
        public static readonly DependencyProperty HeaderTemplateSelectorProperty;


        static DesignerScrollViewer()
        {
            HeaderProperty = DependencyProperty.Register("Header", typeof(object), typeof(DesignerScrollViewer), new FrameworkPropertyMetadata(null, new PropertyChangedCallback(DesignerScrollViewer.OnHeaderChanged)));
            HasHeaderPropertyKey = DependencyProperty.RegisterReadOnly("HasHeader", typeof(bool), typeof(DesignerScrollViewer), new FrameworkPropertyMetadata(BooleanBoxes.FalseBox));
            HasHeaderProperty = HasHeaderPropertyKey.DependencyProperty;
            HeaderTemplateProperty = DependencyProperty.Register("HeaderTemplate", typeof(DataTemplate), typeof(DesignerScrollViewer), new FrameworkPropertyMetadata(null, new PropertyChangedCallback(DesignerScrollViewer.OnHeaderTemplateChanged)));
            HeaderTemplateSelectorProperty = DependencyProperty.Register("HeaderTemplateSelector", typeof(DataTemplateSelector), typeof(DesignerScrollViewer), new FrameworkPropertyMetadata(null, new PropertyChangedCallback(DesignerScrollViewer.OnHeaderTemplateSelectorChanged)));
            HeaderStringFormatProperty = DependencyProperty.Register("HeaderStringFormat", typeof(string), typeof(DesignerScrollViewer), new FrameworkPropertyMetadata(null, new PropertyChangedCallback(DesignerScrollViewer.OnHeaderStringFormatChanged)));
            FrameworkElement.DefaultStyleKeyProperty.OverrideMetadata(typeof(DesignerScrollViewer), new FrameworkPropertyMetadata(typeof(DesignerScrollViewer)));
        }

        protected virtual void OnHeaderChanged(object oldHeader, object newHeader)
        {
            base.RemoveLogicalChild(oldHeader);
            base.AddLogicalChild(newHeader);
        }

        private static void OnHeaderChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            DesignerScrollViewer control = (DesignerScrollViewer)d;
            control.SetValue(HasHeaderPropertyKey, (e.NewValue != null) ? BooleanBoxes.TrueBox : BooleanBoxes.FalseBox);
            control.OnHeaderChanged(e.OldValue, e.NewValue);
        }

        protected virtual void OnHeaderStringFormatChanged(string oldHeaderStringFormat, string newHeaderStringFormat)
        {
        }

        protected virtual void OnHeaderTemplateChanged(DataTemplate oldHeaderTemplate, DataTemplate newHeaderTemplate)
        {
            
        }

        protected virtual void OnHeaderTemplateSelectorChanged(DataTemplateSelector oldHeaderTemplateSelector, DataTemplateSelector newHeaderTemplateSelector)
        {
            
        }

        private static void OnHeaderStringFormatChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            ((DesignerScrollViewer)d).OnHeaderStringFormatChanged((string)e.OldValue, (string)e.NewValue);
        }

        private static void OnHeaderTemplateChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            ((DesignerScrollViewer)d).OnHeaderTemplateChanged((DataTemplate)e.OldValue, (DataTemplate)e.NewValue);
        }

        private static void OnHeaderTemplateSelectorChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            ((DesignerScrollViewer)d).OnHeaderTemplateSelectorChanged((DataTemplateSelector)e.OldValue, (DataTemplateSelector)e.NewValue);
        }

        [Bindable(false), Browsable(false)]
        public bool HasHeader
        {
            get
            {
                return (bool)base.GetValue(HasHeaderProperty);
            }
        }
 

        [Bindable(true), Category("Content"), Localizability(LocalizationCategory.Label)]
        public object Header
        {
            get
            {
                return base.GetValue(HeaderProperty);
            }
            set
            {
                base.SetValue(HeaderProperty, value);
            }
        }
 
        [Category("Content"), Bindable(true)]
        public string HeaderStringFormat
        {
            get
            {
                return (string)base.GetValue(HeaderStringFormatProperty);
            }
            set
            {
                base.SetValue(HeaderStringFormatProperty, value);
            }
        }

        [Category("Content"), Bindable(true)]
        public DataTemplate HeaderTemplate
        {
            get
            {
                return (DataTemplate)base.GetValue(HeaderTemplateProperty);
            }
            set
            {
                base.SetValue(HeaderTemplateProperty, value);
            }
        }

        [Bindable(true), Category("Content")]
        public DataTemplateSelector HeaderTemplateSelector
        {
            get
            {
                return (DataTemplateSelector)base.GetValue(HeaderTemplateSelectorProperty);
            }
            set
            {
                base.SetValue(HeaderTemplateSelectorProperty, value);
            }
        }
        
 

        //protected internal override IEnumerator LogicalChildren { get; } 

    }


    public class DesignerCanvasCommands
    {
        private static RoutedCommand ToggleConnectionsCommand = new RoutedCommand("ToggleConnections",
            typeof(DesignerCanvas));

        private static RoutedCommand ToggleGridLinesCommand = new RoutedCommand("ToggleGridLines",
            typeof(DesignerCanvas));

        private static RoutedCommand IncreaseZoomCommand = new RoutedCommand("IncreaseZoom",
            typeof(DesignerCanvas));

        private static RoutedCommand DecreaseZoomCommand = new RoutedCommand("DecreaseZoom",
            typeof(DesignerCanvas));

        private static RoutedCommand SearchCommand = new RoutedCommand("Search",
            typeof(DesignerCanvas), new InputGestureCollection() {
                new KeyGesture(Key.F3, ModifierKeys.None) });

        private static RoutedCommand RefreshCommand = new RoutedCommand("Refresh",
            typeof(DesignerCanvas));


        public static RoutedCommand IncreaseZoom
        {
            get { return DesignerCanvasCommands.IncreaseZoomCommand; }
        }

        public static RoutedCommand DecreaseZoom
        {
            get { return DesignerCanvasCommands.DecreaseZoomCommand; }
        }

        public static RoutedCommand ToggleGridLines
        {
            get { return DesignerCanvasCommands.ToggleGridLinesCommand; }
        }


        public static RoutedCommand ToggleConnections
        {
            get { return DesignerCanvasCommands.ToggleConnectionsCommand; }
        }

        public static RoutedCommand Search
        {
            get { return DesignerCanvasCommands.SearchCommand; }
        }

        public static RoutedCommand Refresh
        {
            get { return DesignerCanvasCommands.RefreshCommand; }
        }
    }

    public class RangeObservableCollection<T> : System.Collections.ObjectModel.ObservableCollection<T>
    {
        private bool _suppressNotification = false;

        protected override void OnCollectionChanged(System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            if (!_suppressNotification)
                base.OnCollectionChanged(e);
        }

        public void AddRange(IEnumerable<T> list)
        {
            if (list == null)
                throw new ArgumentNullException("list");

            _suppressNotification = true;
            foreach (T item in list) {
                Add(item);
            }
            _suppressNotification = false;
            OnCollectionChanged(new System.Collections.Specialized.NotifyCollectionChangedEventArgs(
                System.Collections.Specialized.NotifyCollectionChangedAction.Reset));
        }
    }

    [TemplatePart(Name = "PART_ScrollViewer", Type = typeof(ScrollViewer))]
    public class DesignerCanvas : MultiSelector
    {
        #region Private Members

        private bool curvedlines = true;
        private int zorder = 1;
        private Point? point;
        private ScrollViewer scrollViewer;
        private TextBlock labelTextBlock;
        protected Dictionary<object, double> canvasLeftLookup = new Dictionary<object, double>();
        protected Dictionary<object, double> canvasTopLookup = new Dictionary<object, double>();        

        public int ZIndex
        {
            get { return zorder; }
            set { zorder = value; }
        }

        #endregion

        #region Public Members

        protected bool _loading = false;
        public event EventHandler OnItemDragCompleted;
        public event EventHandler OnItemDragStarted;
        public event LinkChangedEventHandler ConnectionLinked;
        public event LinkChangedEventHandler ConnectionUnlinked;
        public event LinkPriorityChangedEventHandler ConnectionPriorityChanged;
        public event LinkCanEstablishEventHandler ConnectionCanLink;
        public event PropertyChangedEventHandler ItemPropertyChanged;
        public event System.Collections.Specialized.NotifyCollectionChangedEventHandler ItemsChanged;
        public event ItemDoubleClickedEventHandler ItemDoubleClicked;
        public static readonly DependencyProperty HeaderProperty;
        public static readonly DependencyProperty ZoomProperty;
        

        #endregion

        #region Public Properties

        public IEnumerable<DesignerConnection> Connections
        {
            get
            {
                foreach (object conn in /*ConnectionLayer.Children*/ this.MyConnections)
                {
                    DesignerConnection conn2 = conn as DesignerConnection;
                    if (conn2 == null) continue;
                    yield return conn2;
                }
            }
        }

        /*
        public Canvas ConnectionLayer
        {
            get
            {
                return GetTemplateChild("Part_Connections") as Canvas;
            }
        } 
         * */

        public double HorizontalScrollOffset
        {
            get
            {
                return scrollViewer.HorizontalOffset;
            }
        }

        public double VerticalScrollOffset
        {
            get
            {
                return scrollViewer.VerticalOffset;
            }
        }

        public bool ShowConnections
        {
            get
            {
                return Convert.ToBoolean(GetValue(ShowConnectionsProperty));
            }
            set
            {
                SetValue(ShowConnectionsProperty, value);
            }
        }

        public bool ShowGrid
        {
            get
            {
                return Convert.ToBoolean(GetValue(ShowGridProperty));
            }
            set
            {
                SetValue(ShowGridProperty, value);
            }
        }

        public object Header
        {
            get { return GetValue(DesignerCanvas.HeaderProperty); }
            set { SetValue(DesignerCanvas.HeaderProperty, value); }
        }

        public double Zoom
        {
            get { return (double)GetValue(DesignerCanvas.ZoomProperty); }
            set { SetValue(DesignerCanvas.ZoomProperty, value); }
        }

        public bool CurvedLines
        {
            get
            {
                return curvedlines;
            }
            set
            {
                curvedlines = value;
            }
        }

        public static readonly DependencyProperty ShowConnectionsProperty =
            DependencyProperty.Register("ShowConnections", typeof(bool), typeof(DesignerCanvas), new PropertyMetadata() { DefaultValue = true });
        public static readonly DependencyProperty ShowGridProperty =
            DependencyProperty.Register("ShowGrid", typeof(bool), typeof(DesignerCanvas), new PropertyMetadata() { DefaultValue = true });

        #endregion

        #region Private Methods

        internal object OnBringItemIntoView(object arg)
        {
            FrameworkElement element = this.ItemContainerGenerator.ContainerFromItem(arg) as FrameworkElement;
            if (element != null)
            {
                element.BringIntoView(new Rect(0, 0, (this.RenderSize.Width / 2) + (element.RenderSize.Width / 2), (this.RenderSize.Height / 2) + (element.RenderSize.Height / 2)));
            }
            return null;
        }

        private void ipropertyInterface_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (ItemPropertyChanged != null)
            {
                ItemPropertyChanged.Invoke(sender, e);
            }
        }
        private T GetTemplateChild<T>(string childName) where T : DependencyObject
        {

            T childControl = this.GetTemplateChild(childName) as T;

            if (childControl == null)
            {

                throw new Exception(string.Format("Couldn't find {0}", childName));

            }

            return childControl;

        }

        protected override bool IsItemItsOwnContainerOverride(object item)
        {
            return (item is IDesignerElement);
        }
        protected override DependencyObject GetContainerForItemOverride()
        {
            return new DesignerItem();
        }
        protected override void ClearContainerForItemOverride(DependencyObject element, object item)
        {
            DesignerItem realItem = element as DesignerItem;

            INotifyPropertyChanged ipropertyInterface = (item as INotifyPropertyChanged);
            if( ipropertyInterface != null ) ipropertyInterface.PropertyChanged -= new PropertyChangedEventHandler(ipropertyInterface_PropertyChanged);
            canvasLeftLookup.Remove(item);
            canvasTopLookup.Remove(item);




            var a = (from DesignerConnection connection in Connections
                     where (connection.Source.ParentDesignerItem == realItem || connection.Sink.ParentDesignerItem == realItem)
                     select connection).ToArray();

            foreach (DesignerConnection connection in a)
            {
                if (connection.Source.ParentDesignerItem == element)
                {
                    object item2 = ItemContainerGenerator.ItemFromContainer(connection.Sink.ParentDesignerItem);
                    LinkEventArgs args = new LinkEventArgs();
                    args.Source = item;
                    args.Target = item2;
                    args.SourceConnector = connection.Source;
                    args.TargetConnector = connection.Sink;
                    if (ConnectionUnlinked != null) ConnectionUnlinked.Invoke(this, args);
                }
                else
                {
                    object item2 = ItemContainerGenerator.ItemFromContainer(connection.Source.ParentDesignerItem);
                    LinkEventArgs args = new LinkEventArgs();
                    args.Source = item2;
                    args.Target = item;
                    args.SourceConnector = connection.Sink;
                    args.TargetConnector = connection.Source;
                    if (ConnectionUnlinked != null) ConnectionUnlinked.Invoke(this, args);
                }



                //NotifyConnectionUnlink(connection.Source, connection.Sink);
                //ConnectionLayer.Children.Remove(connection);
                MyConnections.Remove(connection);
            }


            base.ClearContainerForItemOverride(element, item);
        }
        protected override void PrepareContainerForItemOverride(DependencyObject element, object item)
        {
            if (element is DesignerItem)
            {
                DesignerItem contentitem = element as DesignerItem;
                contentitem.SetBinding(ResizeableCanvas.LeftProperty, new Binding("Left") { Mode = BindingMode.TwoWay, Source = contentitem });
                contentitem.SetBinding(ResizeableCanvas.TopProperty, new Binding("Top") { Mode = BindingMode.TwoWay, Source = contentitem });
                contentitem.SetBinding(ResizeableCanvas.ZIndexProperty, new Binding("ZIndex") { Mode = BindingMode.TwoWay, Source = contentitem });
                INotifyPropertyChanged ipropertyInterface = (item as INotifyPropertyChanged);
                if (ipropertyInterface != null) ipropertyInterface.PropertyChanged += new PropertyChangedEventHandler(ipropertyInterface_PropertyChanged);
                base.PrepareContainerForItemOverride(element, item);


                DesignerItem designerItem = ItemContainerGenerator.ContainerFromItem(item) as DesignerItem;
                if (canvasLeftLookup.ContainsKey(item))
                {
                    contentitem.SetValue(DesignerItem.LeftProperty, canvasLeftLookup[item]);
                    canvasLeftLookup.Remove(item);
                }

                if (canvasTopLookup.ContainsKey(item))
                {
                    contentitem.SetValue(DesignerItem.TopProperty, canvasTopLookup[item]);
                    canvasTopLookup.Remove(item);
                }

                contentitem.SetValue(DesignerItem.ZIndexProperty, ZIndex++);
                contentitem.Loaded += new RoutedEventHandler(contentitem_Loaded);
            }            
        }

        protected virtual void contentitem_Loaded(object sender, RoutedEventArgs e)
        {
            
        }




        protected override void OnItemsChanged(System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            
            if (e.Action == System.Collections.Specialized.NotifyCollectionChangedAction.Remove)
            {
                List<DesignerConnection> connections = new List<DesignerConnection>();
                foreach (object c in e.OldItems)
                {
                    object d = ItemContainerGenerator.ContainerFromItem(c);
                    foreach (DesignerConnection conn in MyConnections)
                    {
                        if (conn.SourceElement == d || conn.SinkElement == d)
                            connections.Add(conn);
                    }
                }

                foreach( DesignerConnection conn in connections )
                    MyConnections.Remove(conn);
            }


            base.OnItemsChanged(e);
            if (_loading == false)
            {
                if (ItemsChanged != null)
                    ItemsChanged.Invoke(this, e);
            }
        }

        
        Point? grabPoint = null;

        protected override void OnMouseLeftButtonUp(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonUp(e);

            if (Cursor == Cursors.ScrollAll && grabPoint.HasValue)
            {
                Vector diff = Mouse.GetPosition(this) - grabPoint.Value;
                scrollViewer.ScrollToHorizontalOffset(scrollViewer.HorizontalOffset + diff.X);
                scrollViewer.ScrollToVerticalOffset(scrollViewer.VerticalOffset + diff.Y);
                grabPoint = null;
                Cursor = Cursors.Arrow;
            }
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            if (Keyboard.Modifiers == ModifierKeys.None)
            {
                grabPoint = Mouse.GetPosition(this);
                SelectedItem = null;
                Cursor = Cursors.ScrollAll;
                Focus();
            }
        }

        protected override void OnMouseRightButtonDown(MouseButtonEventArgs e)
        {
            if ((Keyboard.Modifiers & ModifierKeys.Shift) == ModifierKeys.Shift)
                point = e.GetPosition(this);
            base.OnMouseRightButtonDown(e);
        }

        protected override void OnMouseRightButtonUp(MouseButtonEventArgs e)
        {
            //e.Handled = true;
            base.OnMouseRightButtonUp(e);
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            if (grabPoint.HasValue && Cursor == Cursors.ScrollAll)
            {
                Vector diff = Mouse.GetPosition(this) - grabPoint.Value;
                scrollViewer.ScrollToHorizontalOffset(scrollViewer.HorizontalOffset + diff.X);
                scrollViewer.ScrollToVerticalOffset(scrollViewer.VerticalOffset + diff.Y);
                grabPoint = Mouse.GetPosition(this);
            }

            if (e.RightButton == MouseButtonState.Pressed )
            {
                AdornerLayer adornerLayer = AdornerLayer.GetAdornerLayer(this);
                if (adornerLayer != null)
                {
                    SelectionAdorner adorner = new SelectionAdorner(this);
                    if (adorner != null)
                    {
                        adornerLayer.Add(adorner);
                        e.Handled = true;
                    }
                }
            }

            base.OnMouseMove(e);
        }

        internal void NotifyDesignerItemDoubleClicked(DesignerItem designerItem)
        {
            object item = base.ItemContainerGenerator.ItemFromContainer(designerItem);
            if (item != null)
            {
                if (ItemDoubleClicked != null) ItemDoubleClicked.Invoke(this, item);
            }
        }

        internal void NotifyDesignerItemClicked(IDesignerElement designerItem, MouseButton mouseButton)
        {
            object item = base.ItemContainerGenerator.ItemFromContainer(designerItem as DependencyObject);
            if (item != null)
            {
                if (Keyboard.Modifiers == ModifierKeys.None || Keyboard.Modifiers == ModifierKeys.Alt || Keyboard.Modifiers == ModifierKeys.Windows)
                    SelectedItems.Clear();
                if (Keyboard.Modifiers == ModifierKeys.Control) {
                    SelectedItems.Remove(item);
                } else {
                    SelectedItems.Add(item);
                }                
            }
        }
        internal static bool UiGetIsSelectable(DependencyObject o)
        {
            if (o != null)
            {
                return true;
            }
            return false;
        }


        internal void NotifyDragCompleted()
        {
            try
            {
                if (OnItemDragCompleted != null)
                    OnItemDragCompleted.Invoke(this, EventArgs.Empty);
            }
            catch (Exception ex)
            {
            }
        }

        internal void NotifyDragStarted()
        {
            try
            {
                if (OnItemDragStarted != null)
                    OnItemDragStarted.Invoke(this, EventArgs.Empty);
            }
            catch (Exception ex)
            {
            }
        }


        internal void NotifyConnectionLink(Connector source, Connector sink)
        {

            object item1 = ItemContainerGenerator.ItemFromContainer((source.ParentDesignerItem as DesignerItem));
            object item2 = ItemContainerGenerator.ItemFromContainer((sink.ParentDesignerItem as DesignerItem));

            try
            {
                LinkEventArgs args = new LinkEventArgs();
                args.Source = item1;
                args.SourceConnector = source;
                args.Target = item2;
                args.TargetConnector = sink;
                if (ConnectionLinked != null) ConnectionLinked.Invoke(this, args);
            }
            catch (Exception ex)
            {

            }
        }
        internal void NotifyConnectionUnlink(Connector source, Connector sink)
        {
            object item1 = ItemContainerGenerator.ItemFromContainer((source.ParentDesignerItem as DesignerItem));
            object item2 = ItemContainerGenerator.ItemFromContainer((sink.ParentDesignerItem as DesignerItem));

            try
            {
                LinkEventArgs args = new LinkEventArgs();
                args.Source = item1;
                args.SourceConnector = source;
                args.Target = item2;
                args.TargetConnector = sink;
                if (ConnectionUnlinked != null) ConnectionUnlinked.Invoke(this, args);
            }
            catch (Exception ex)
            {

            }
        }
        internal void NotifyConnectionPriority(DesignerItem source, DesignerItem sink)
        {
            object item1 = ItemContainerGenerator.ItemFromContainer(source);
            object item2 = ItemContainerGenerator.ItemFromContainer(sink);

            try
            {
                if (ConnectionPriorityChanged != null)
                {
                    double left = ResizeableCanvas.GetLeft(source);
                    ConnectionPriorityChanged.Invoke(this, item1, item2, (float)left);
                }
            }
            catch (Exception ex)
            {

            }
        }
        internal bool IsLinkable(Connector source, Connector sink)
        {
            object item1 = ItemContainerGenerator.ItemFromContainer((source.ParentDesignerItem as DesignerItem));
            object item2 = ItemContainerGenerator.ItemFromContainer((sink.ParentDesignerItem as DesignerItem));

            try
            {
                LinkCanEstablishEventArgs args = new LinkCanEstablishEventArgs();
                args.Source = item1;
                args.SourceConnector = source;
                args.Target = item2;
                args.TargetConnector = sink;
                args.IsAllowed = true;
                if (ConnectionCanLink != null) ConnectionCanLink.Invoke(this, args);
                return args.IsAllowed;
            }
            catch (Exception ex)
            {
                return false;
            }
        }

        #endregion

        #region Public Methods

        protected virtual void Refresh()
        {
        }

        public Point Transform(Connector a, Point p)
        {
            //return a.TransformToAncestor(GetTemplateChild("Part_Grid") as Visual).Transform(p);
            return a.TranslatePoint(p, (GetTemplateChild("Items") as UIElement));
        }

        public void ScrollIntoView(object item)
        {
            if (base.ItemContainerGenerator.Status == GeneratorStatus.ContainersGenerated)
            {
                OnBringItemIntoView(item);
            }
            else
            {
                base.Dispatcher.BeginInvoke(DispatcherPriority.Loaded, new DispatcherOperationCallback(this.OnBringItemIntoView), item);
            }
        }

        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();
            this.scrollViewer = this.GetTemplateChild<ScrollViewer>("PART_ScrollViewer");
        }


        public object[] FindConnections(object item)
        {
            List<object> connections = new List<object>();
            foreach (DesignerConnection connection in Connections)
            {
                DesignerItem item2 = UIHelper.FindAnchestor<DesignerItem>(connection.Source);
                DesignerItem item3 = UIHelper.FindAnchestor<DesignerItem>(connection.Sink);
                object c = ItemContainerGenerator.IndexFromContainer(item2);
                object d = ItemContainerGenerator.IndexFromContainer(item2);

                if (c == item)
                {
                    connections.Add(d);
                }

                if (d == item)
                {
                    connections.Add(c);
                }
            }

            return connections.Distinct().ToArray();
        }



        #endregion

        #region Ctor/Dtor

        static DesignerCanvas()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(DesignerCanvas), new FrameworkPropertyMetadata(typeof(DesignerCanvas)));

            HeaderProperty = DependencyProperty.Register("Header", typeof(object), typeof(DesignerCanvas),
                    new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.None));
            ZoomProperty = DependencyProperty.Register("Zoom", typeof(double), typeof(DesignerCanvas),
                    new FrameworkPropertyMetadata( (double)1.0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));

            

            

            
            


            CommandManager.RegisterClassCommandBinding(typeof(DesignerCanvas), new CommandBinding(ApplicationCommands.Delete, CommandDeleteHandler));
            CommandManager.RegisterClassCommandBinding(typeof(DesignerCanvas), new CommandBinding(ApplicationCommands.Copy, CommandCopyHandler));
            CommandManager.RegisterClassCommandBinding(typeof(DesignerCanvas), new CommandBinding(ApplicationCommands.Cut, CommandCutHandler));
            CommandManager.RegisterClassCommandBinding(typeof(DesignerCanvas), new CommandBinding(ApplicationCommands.Paste, CommandPasteHandler));
            CommandManager.RegisterClassCommandBinding(typeof(DesignerCanvas), new CommandBinding(ApplicationCommands.SelectAll, CommandSelectAllHandler));
            CommandManager.RegisterClassCommandBinding(typeof(DesignerCanvas), new CommandBinding(ApplicationCommands.Find, CommandSearchHandler));
            CommandManager.RegisterClassCommandBinding(typeof(DesignerCanvas), new CommandBinding(ComponentCommands.MoveDown, CommandMoveDownHandler));
            CommandManager.RegisterClassCommandBinding(typeof(DesignerCanvas), new CommandBinding(ComponentCommands.MoveUp, CommandCopyHandler));
            CommandManager.RegisterClassCommandBinding(typeof(DesignerCanvas), new CommandBinding(ComponentCommands.MoveLeft, CommandCutHandler));
            CommandManager.RegisterClassCommandBinding(typeof(DesignerCanvas), new CommandBinding(ComponentCommands.MoveRight, CommandPasteHandler));            
            CommandManager.RegisterClassCommandBinding(typeof(DesignerCanvas), new CommandBinding(DesignerCanvasCommands.ToggleGridLines, CommandToggleGridLinesHandler));
            CommandManager.RegisterClassCommandBinding(typeof(DesignerCanvas), new CommandBinding(DesignerCanvasCommands.ToggleConnections, CommandToggleConnectionsHandler));            
            CommandManager.RegisterClassCommandBinding(typeof(DesignerCanvas), new CommandBinding(DesignerCanvasCommands.Search, CommandSearchHandler));
            CommandManager.RegisterClassCommandBinding(typeof(DesignerCanvas), new CommandBinding(DesignerCanvasCommands.IncreaseZoom, CommandZoomHandler));
            CommandManager.RegisterClassCommandBinding(typeof(DesignerCanvas), new CommandBinding(DesignerCanvasCommands.DecreaseZoom, CommandZoomHandler));
            CommandManager.RegisterClassCommandBinding(typeof(DesignerCanvas), new CommandBinding(DesignerCanvasCommands.Refresh, CommandRefreshHandler));
            
            
            

            

        }
        

        private static void CommandRefreshHandler(object target, ExecutedRoutedEventArgs e)
        {
            DesignerCanvas canvas = (target as DesignerCanvas);
            if (e.Command == DesignerCanvasCommands.Refresh)
            {
                canvas.Refresh();
            }
        }


        private static void CommandZoomHandler(object target, ExecutedRoutedEventArgs e)
        {
            DesignerCanvas canvas = (e.Source as DesignerCanvas);
            if (e.Command == DesignerCanvasCommands.IncreaseZoom)
            {
                double zoom = canvas.Zoom + 0.05;
                if (zoom > 1) zoom = 1;
                canvas.Zoom = zoom;
            }
            else if (e.Command == DesignerCanvasCommands.DecreaseZoom)
            {
                double zoom = canvas.Zoom - 0.05;
                if (zoom < 0.1) zoom = 0.1;
                canvas.Zoom = zoom;
            }
        }

        private static void CommandSearchHandler(object target, ExecutedRoutedEventArgs e)
        {
            DesignerCanvas canvas = (e.Source as DesignerCanvas);
            TextBox searchPart = canvas.GetTemplateChild("Part_Search") as TextBox;
            CheckBox searchCasePart = canvas.GetTemplateChild("Part_SearchCaseSensitive") as CheckBox;
            if (searchPart == null)
                return;
            if (searchCasePart == null)
                return;            

            if (e.Command == ApplicationCommands.Find)
            {
                searchPart.Focus();
                Keyboard.Focus(searchPart);
            }
            else if (e.Command == DesignerCanvasCommands.Search)
            {
                if (canvas.Items.Count == 0)
                    return;

                bool caseSensitive = searchCasePart.IsChecked.HasValue && searchCasePart.IsChecked.Value;
                canvas.Items.MoveCurrentTo(canvas.SelectedItem);
                int cursor = canvas.Items.CurrentPosition;
                string text = searchPart.Text;
                for (int i = canvas.Items.CurrentPosition + 1; i < canvas.Items.Count; i++)
                {
                    if (i > -1)
                    {
                        object c = canvas.Items.GetItemAt(i);
                        DependencyObject d = canvas.ItemContainerGenerator.ContainerFromItem(c);
                        if (FindText(d, text, caseSensitive, 0))
                        {
                            canvas.UnselectAll();
                            canvas.SelectedIndex = i;
                            canvas.ScrollIntoView(c);
                            return;
                        }
                    }
                }

                for (int i = 0; i < (cursor + 1); i++)
                {
                    if (i > -1)
                    {
                        object c = canvas.Items.GetItemAt(i);
                        DependencyObject d = canvas.ItemContainerGenerator.ContainerFromItem(c);
                        if (FindText(d, text, caseSensitive, 0))
                        {
                            canvas.UnselectAll();
                            canvas.SelectedIndex = i;
                            canvas.ScrollIntoView(c);
                            return;
                        }
                    }
                }
            }                                
        }

        private static bool FindText(DependencyObject d, string text, bool caseSensitive, int level)
        {
            if (level == 0 && !caseSensitive)
            {
                text = text.ToLower();
            }

            int max = VisualTreeHelper.GetChildrenCount(d);
            for (int i = 0; i < max; i++) 
            {
                DependencyObject dc = VisualTreeHelper.GetChild(d, i);
                TextBox partTextbox = dc as TextBox;
                Label   partLabel = dc as Label;
                TextBlock partTextBlock = dc as TextBlock;

                if (partTextBlock != null)
                {
                    if (caseSensitive)
                    {                        
                        bool retval = partTextBlock.Text.Contains(text);
                        if (retval) return true; 
                    }
                    else
                    {
                        bool retval = partTextBlock.Text.ToLower().Contains(text);
                        if (retval) return true;     
                    }
                }
                else if (partLabel != null)
                {
                    if (caseSensitive)
                    {                        
                        bool retval = partLabel.Content.ToString().Contains(text);
                        if (retval) return true;
                    }
                    else
                    {
                        bool retval = partLabel.Content.ToString().ToLower().Contains(text);
                        if (retval) return true;   
                    }
                }
                else if (partTextBlock != null)
                {
                    if (caseSensitive)
                    {
                        bool retval = partTextBlock.Text.Contains(text);
                        if (retval) return true;
                    }
                    else
                    {
                        bool retval = partTextBlock.Text.ToLower().Contains(text);
                        if (retval) return true;
                    }
                }
                else
                {
                    bool retval = FindText(dc, text, caseSensitive, level + 1);
                    if (retval) return true;
                }                
            }

            return false;
        }


        private static void CommandSelectAllHandler(object target, ExecutedRoutedEventArgs e)
        {
            DesignerCanvas canvas = (e.Source as DesignerCanvas);
            canvas.SelectAll();
        }

        private static void CommandMoveDownHandler(object target, ExecutedRoutedEventArgs e)
        {
            DesignerCanvas canvas = (e.Source as DesignerCanvas);
            object obj = canvas.SelectedItem;
            if (obj != null)
            {
                DesignerItem designItem = canvas.ItemContainerGenerator.ContainerFromItem(obj) as DesignerItem;
                designItem.SetValue(DesignerItem.LeftProperty, Convert.ToDouble(designItem.GetValue(DesignerItem.LeftProperty)) + 10);
                designItem.SetValue(DesignerItem.TopProperty, Convert.ToDouble(designItem.GetValue(DesignerItem.TopProperty)) + 10);
            }

        }

        private static void CommandDeleteHandler(object target, ExecutedRoutedEventArgs e)
        {
            DesignerCanvas canvas = (e.Source as DesignerCanvas);
            foreach (object obj in canvas.SelectedItems.OfType<object>().ToArray())
            {
                try
                {
                    canvas.Items.Remove(obj);
                }
                catch (Exception ex)
                {
                }
            }
        }

        private static void CommandCopyHandler(object target, ExecutedRoutedEventArgs e)
        {
            DesignerCanvas canvas = (e.Source as DesignerCanvas);
            DesignerItem designItem = canvas.ItemContainerGenerator.ContainerFromItem(canvas.SelectedItem) as DesignerItem;
            DesignerComment designComment = canvas.ItemContainerGenerator.ContainerFromItem(canvas.SelectedItem) as DesignerComment;
            MetaData meta = new MetaData();

            if (designItem != null)
            {                
                meta.left = (double)(designItem.GetValue(DesignerItem.LeftProperty));
                meta.top = (double)(designItem.GetValue(DesignerItem.TopProperty));
                meta.item = canvas.SelectedItem;
                Clipboard.SetData("DesignerCanvas", meta);
            }
            else
            {
                
                meta.left = ResizeableCanvas.GetLeft(designComment);
                meta.top = ResizeableCanvas.GetTop(designComment);
                meta.item = canvas.SelectedItem;
                Clipboard.SetData("DesignerCanvas", meta);
            }
        }

        private static void CommandCutHandler(object target, ExecutedRoutedEventArgs e)
        {
            CommandCopyHandler(target, e);

            DesignerCanvas canvas = (e.Source as DesignerCanvas);
            canvas.Items.Remove(canvas.SelectedItem);
        }

        private static void CommandPasteHandler(object target, ExecutedRoutedEventArgs e)
        {
            try
            {

                object o = Clipboard.GetData("DesignerCanvas");
                if (o != null)
                {
                    DesignerCanvas canvas = (e.Source as DesignerCanvas);
                    MetaData meta = (MetaData)o;
                    canvas.Items.Add(meta.item);

                    if (canvas.ItemContainerGenerator.ContainerFromItem(meta.item) is DesignerItem)
                    {
                        ResizeableCanvas.SetLeft(canvas.ItemContainerGenerator.ContainerFromItem(meta.item) as DesignerItem, meta.left + 10);
                        ResizeableCanvas.SetTop(canvas.ItemContainerGenerator.ContainerFromItem(meta.item) as DesignerItem, meta.top + 10);
                    }
                    else if (canvas.ItemContainerGenerator.ContainerFromItem(meta.item) is DesignerComment)
                    {
                        ResizeableCanvas.SetLeft(canvas.ItemContainerGenerator.ContainerFromItem(meta.item) as DesignerComment, meta.left + 10);
                        ResizeableCanvas.SetTop(canvas.ItemContainerGenerator.ContainerFromItem(meta.item) as DesignerComment, meta.top + 10);
                    }
                    
                    canvas.SelectedItem = meta.item;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error");
            }
        }

        private static void CommandToggleGridLinesHandler(object target, ExecutedRoutedEventArgs e)
        {
            DesignerCanvas canvas = (e.Source as DesignerCanvas);
            canvas.ShowGrid ^= true;
        }

        private static void CommandToggleConnectionsHandler(object target, ExecutedRoutedEventArgs e)
        {
            DesignerCanvas canvas = (e.Source as DesignerCanvas);
            canvas.ShowConnections ^= true;
        }

        

        public DesignerCanvas()
        {

        }

        #endregion

        #region Nested

        [Serializable()]
        struct MetaData
        {
            public double left;
            public double top;
            public object item;
        }

        #endregion

        //System.Collections.ObjectModel.ObservableCollection<DesignerConnection> MyConnections;

        internal RangeObservableCollection<DesignerConnection> MyConnections = 
            new RangeObservableCollection<DesignerConnection>();        
    }


    public class DesignerConnectionVisual : DrawingVisual
    {
        public DesignerConnectionVisual(Dictionary<DesignerConnection, Transform> connections, DesignerCanvas Canvas, Geometry Data)
        {
            using (DrawingContext drawingContext = RenderOpen())
            {
                Geometry d = Data.Clone(); d.Freeze();
                Pen p = new Pen();
                p.Brush = Brushes.Black;
                p.StartLineCap = PenLineCap.Round;
                p.EndLineCap = PenLineCap.Round;
                p.LineJoin = PenLineJoin.Round;
                p.DashCap = PenLineCap.Round;
                p.Thickness = 1.2;
                p.DashStyle = new DashStyle();
                p.DashStyle.Dashes.Add(1.9);
                p.Freeze();

                Pen p2 = new Pen(Brushes.Gold, 3.0);
                p2.Freeze();


                foreach (KeyValuePair<DesignerConnection, Transform> pair in connections)
                {
                    drawingContext.DrawGeometry(null, p, pair.Key.PathGeometry);
                    
                }

                foreach (KeyValuePair<DesignerConnection, Transform> pair in connections)
                {
                    drawingContext.PushTransform(pair.Value);
                    drawingContext.DrawGeometry(Brushes.Green, null, d);
                    drawingContext.Pop();
                }
            }
        }
    }

    public class DesignerConnectionPresenter : FrameworkElement 
    {               
        DesignerCanvas Canvas = null;
        Dictionary<PathFigureCollection, DesignerConnection> connections2 = new Dictionary<PathFigureCollection, DesignerConnection>();
        Dictionary<DesignerConnection, Transform> connections = new Dictionary<DesignerConnection, Transform>();
        DrawingVisual visuals = new DrawingVisual();

        public static readonly DependencyProperty DataProperty;

        static DesignerConnectionPresenter()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(DesignerConnectionPresenter), 
                new FrameworkPropertyMetadata(typeof(DesignerConnectionPresenter)));
            DataProperty = DependencyProperty.Register("Data", typeof(Geometry), typeof(DesignerConnectionPresenter),
                    new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.None));
        }

        public DesignerConnectionPresenter()
        {
            Loaded += new RoutedEventHandler(LoadedCore);
        }

        public Geometry Data
        {
            get
            {
                return (Geometry)GetValue(DataProperty);
            }
            set
            {
                SetValue(DataProperty, value);
            }
        }

        private void LoadedCore(object sender, RoutedEventArgs e)
        {
            DesignerCanvas canvas = FindAnchestor<DesignerCanvas>(this);
            if (Canvas != canvas)
            {
                Canvas = canvas;
                (canvas.MyConnections as System.Collections.Specialized.INotifyCollectionChanged).CollectionChanged += 
                    new System.Collections.Specialized.NotifyCollectionChangedEventHandler(CollectionChangedCore);
            }
        }

        private void CollectionChangedCore(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            ICollection<DesignerConnection> collection = sender as ICollection<DesignerConnection>;
            if (e.Action == System.Collections.Specialized.NotifyCollectionChangedAction.Reset)
            {
                foreach (DesignerConnection a in connections.Select(x => x.Key).ToArray())
                {
                    connections2.Remove(a.PathGeometry.Figures);
                    connections.Remove(a);
                    a.PathGeometry.Figures.Changed -= FiguresChangedCore;
                }

                foreach (DesignerConnection a in collection)
                {
                    TransformGroup g = new TransformGroup();
                    Point point, tangent;
                    a.PathGeometry.GetPointAtFractionLength(0.5, out point, out tangent);
                    Vector v = new Vector(tangent.X, tangent.Y); v.Normalize();
                    double angle = Vector.AngleBetween(new Vector(1, 0), v) + 180;
                    RotateTransform r = new RotateTransform(angle) { CenterX = 5, CenterY = 5 };
                    TranslateTransform t = new TranslateTransform(point.X - 5.5, point.Y - 5.5);
                    g.Children.Add(r);
                    g.Children.Add(a.PathGeometry.Transform);
                    g.Children.Add(t);
                    connections[a] = g;
                    connections2[a.PathGeometry.Figures] = a;
                    a.PathGeometry.Figures.Changed += FiguresChangedCore;
                }
            }           
            else if (e.Action == System.Collections.Specialized.NotifyCollectionChangedAction.Add)
            {
                foreach (DesignerConnection a in e.NewItems)
                {
                    TransformGroup g = new TransformGroup();
                    Point point, tangent;
                    a.PathGeometry.GetPointAtFractionLength(0.5, out point, out tangent);
                    Vector v = new Vector(tangent.X, tangent.Y); v.Normalize();
                    double angle = Vector.AngleBetween(new Vector(1, 0), v) + 180;
                    RotateTransform r = new RotateTransform(angle) { CenterX = 5, CenterY = 5 };
                    TranslateTransform t = new TranslateTransform(point.X - 5.5, point.Y - 5.5);
                    g.Children.Add(r);
                    g.Children.Add(a.PathGeometry.Transform);
                    g.Children.Add(t);
                    connections[a] = g;
                    connections2[a.PathGeometry.Figures] = a;
                    a.PathGeometry.Figures.Changed += FiguresChangedCore;
                }
            }
            else if (e.Action == System.Collections.Specialized.NotifyCollectionChangedAction.Remove)
            {
                foreach (DesignerConnection a in e.OldItems)
                {
                    connections2.Remove(a.PathGeometry.Figures);
                    connections.Remove(a);
                    a.PathGeometry.Figures.Changed -= FiguresChangedCore;
                }
            }

            RemoveVisualChild(visuals);
            visuals = new DesignerConnectionVisual(connections, Canvas, Data);
            AddVisualChild(visuals);
        }

        private void FiguresChangedCore(object sender, EventArgs e)
        {
            DesignerConnection a = connections2[sender as PathFigureCollection];
            TransformGroup g = connections[a] as TransformGroup;
            RotateTransform r = g.Children[0] as RotateTransform;
            TranslateTransform t = g.Children[2] as TranslateTransform;
            Point point, tangent;
            a.PathGeometry.GetPointAtFractionLength(0.5, out point, out tangent);
            Vector v = new Vector(tangent.X, tangent.Y); v.Normalize();
            double angle = Vector.AngleBetween(new Vector(1, 0), v) + 180;
            t.X = point.X - 5.5;
            t.Y = point.Y - 5.5;
            r.Angle = angle;            
        }
     
        protected override void OnVisualParentChanged(DependencyObject oldParent)
        {
            base.OnVisualParentChanged(oldParent);
            DesignerCanvas canvas = FindAnchestor<DesignerCanvas>(this);
            if (Canvas != canvas)
            {
                Canvas = canvas;
                InvalidateVisual();
            }
        }

        protected override Size MeasureOverride(Size availableSize)
        {
            //return new Size(0, 0);

            Size s = base.MeasureOverride(availableSize);
            return s;
        }

        protected override Size ArrangeOverride(Size finalSize)
        {
            Size s = base.ArrangeOverride(finalSize);
            return s;
        }

        protected override Visual GetVisualChild(int index)
        {
            return visuals;
        }

        protected override int VisualChildrenCount
        {
            get
            {
                return 1;
            }
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            if (Canvas == null)
                return;
            base.OnRender(drawingContext);
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

    public class DesignerAdorner : AdornerDecorator
    {
    }

    public class DesignerTransformConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is double)
            {
                double d = (double)value;
                return new ScaleTransform { ScaleY = d, ScaleX = d };
            }
            else
            {
                return new ScaleTransform();
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }


    public class DesignerTransformConverter2 : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            PathGeometry geometry = value as PathGeometry;
            if (geometry != null)
            {
                Point point, tangent;
                geometry.GetPointAtFractionLength(0.5, out point, out tangent);
                TransformGroup g = new TransformGroup();
                Vector v = new Vector(tangent.X, tangent.Y); v.Normalize();
                double angle = Vector.AngleBetween(new Vector(1, 0), v) + 180;
                g.Children.Add(new RotateTransform(angle));
                g.Children.Add(new TranslateTransform(point.X - 5.5, point.Y - 5.5));
                return g;
            }
            else
            {
                return new TransformGroup();
            }

        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    public interface IDesignerElement
    {
    }

    public class DesignerConnection : Control, INotifyPropertyChanged, IDesignerElement
    {
        #region Private Members

        internal int suspendlayout = 0;
        private DesignerCanvas canvas;
        private Connector source;
        private Connector sink;
        private PathGeometry pathGeometry = new PathGeometry() { Transform = new TranslateTransform() { } };
        private static LeftConverter LeftConverterField = new LeftConverter();
        private static TopConverter TopConverterField = new TopConverter();

        #endregion

        #region Public Members

        public static readonly DependencyProperty SourceAnchorTopProperty;
        public static readonly DependencyProperty SourceAnchorLeftProperty;
        public static readonly DependencyProperty SourceAnchorAngleProperty;
        public static readonly DependencyProperty SinkAnchorTopProperty;
        public static readonly DependencyProperty SinkAnchorLeftProperty;
        public static readonly DependencyProperty SinkAnchorAngleProperty;

        #endregion

        #region Public Properties

        public object SourceElement
        {
            get
            {
                if (source != null)
                {
                    return FindDesignerElement(source);
                }
                else
                {
                    return null;
                }
            }
        }

        public object SinkElement
        {
            get
            {
                if (sink != null)
                {
                    return FindDesignerElement(sink);
                }
                else
                {
                    return null;
                }
            }
        }

        public Connector Source
        {
            get
            {
                return source;
            }
            set
            {
                if (source != value)
                {
                    if( source != null )
                        (source as INotifyPropertyChanged).PropertyChanged -= DesignerConnection_PropertyChanged;
                    if (value != null)
                        (value as INotifyPropertyChanged).PropertyChanged += DesignerConnection_PropertyChanged;
                    source = value;

                    /*
                    SetBinding(DesignerConnection.SourceAnchorTopProperty, new Binding("Left") { Mode = BindingMode.OneWay, Source = value.ParentDesignerItem });
                    SetBinding(DesignerConnection.SourceAnchorLeftProperty, new Binding("Top") { Mode = BindingMode.OneWay, Source = value.ParentDesignerItem });                    
                    */
                    UpdatePathGeometry();
                    //isdirty = true;
                }
            }
        }

        public Connector Sink
        {
            get
            {
                return sink;
            }
            set
            {
                if (sink != value)
                {
                    if (sink != null)
                        (sink as INotifyPropertyChanged).PropertyChanged -= DesignerConnection_PropertyChanged;
                    if (value != null)
                        (value as INotifyPropertyChanged).PropertyChanged += DesignerConnection_PropertyChanged;
                    sink = value;

                    //SetBinding(DesignerConnection.SinkAnchorTopProperty, new Binding("Left") { Mode = BindingMode.OneWay, Source = value.ParentDesignerItem });
                    //SetBinding(DesignerConnection.SinkAnchorLeftProperty, new Binding("Top") { Mode = BindingMode.OneWay, Source = value.ParentDesignerItem });
                    UpdatePathGeometry();
                    //isdirty = true;
                }
            }
        }
        public PathGeometry PathGeometry
        {
            get 
            {
                if (suspendlayout == 1)
                {
                    return pathGeometry; 
                }

                if (isdirty)
                {
                    UpdatePathGeometry();
                    isdirty = false;
                }

                return pathGeometry; 
            }
            protected set
            {
                if (pathGeometry != value)
                {
                    pathGeometry = value;
                    if (PropertyChanged != null) PropertyChanged.Invoke(this, new PropertyChangedEventArgs("PathGeometry"));
                }
            }
        }

        public double SourceAnchorAngle
        {
            get { return (double)GetValue(DesignerConnection.SourceAnchorAngleProperty); }
            set { SetValue(DesignerConnection.SourceAnchorAngleProperty, value); }
        }

        public double SinkAnchorAngle
        {
            get { return (double)GetValue(DesignerConnection.SinkAnchorAngleProperty); }
            set { SetValue(DesignerConnection.SinkAnchorAngleProperty, value); }
        }

        public double SourceAnchorTop
        {
            get { return (double)GetValue(DesignerConnection.SourceAnchorTopProperty); }
            set { SetValue(DesignerConnection.SourceAnchorTopProperty, value); }
        }

        public double SourceAnchorLeft
        {
            get { return (double)GetValue(DesignerConnection.SourceAnchorLeftProperty); }
            set { SetValue(DesignerConnection.SourceAnchorLeftProperty, value); }
        }

        public double SinkAnchorTop
        {
            get { return (double)GetValue(DesignerConnection.SinkAnchorTopProperty); }
            set { SetValue(DesignerConnection.SinkAnchorTopProperty, value); }
        }

        public double SinkAnchorLeft
        {
            get { return (double)GetValue(DesignerConnection.SinkAnchorLeftProperty); }
            set { SetValue(DesignerConnection.SinkAnchorLeftProperty, value); }
        }


        #endregion

        #region Private Methods

        private bool isdirty = true;

        private static void OnPositioningChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            /*
            (d as DesignerConnection).UpdatePathGeometry();
            (d as DesignerConnection).UpdateAnchorPosition();
            */
            DesignerCanvas canvas = FindAnchestor<DesignerCanvas>((d as DesignerConnection));
            if (canvas != null)
            {
                canvas.NotifyConnectionPriority((d as DesignerConnection).Source.ParentDesignerItem, (d as DesignerConnection).Sink.ParentDesignerItem);
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

        private static IDesignerElement FindDesignerElement(DependencyObject current)
        {
            do
            {
                if (current is IDesignerElement)
                {
                    return (IDesignerElement)current;
                }
                current = VisualTreeHelper.GetParent(current);
            }
            while (current != null);
            return null;
        }

        void DesignerConnection_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {            
            
            //InvalidateVisual();  

            

            isdirty = true;
            

            if (suspendlayout == 0)
            {
                UpdatePathGeometry();
                /*
                if (PropertyChanged != null)
                    PropertyChanged.Invoke(this, new PropertyChangedEventArgs("PathGeometry"));
                 */
            }

            DesignerCanvas canvas = FindAnchestor<DesignerCanvas>(this);
            if (canvas != null)
            {
                canvas.NotifyConnectionPriority(Source.ParentDesignerItem, Sink.ParentDesignerItem);
            }            
        }

        private void DesignerConnection_Loaded(object sender, RoutedEventArgs e)
        {
            UpdatePathGeometry();
        }

        private void OnConnectorPositionChanged(object sender, PropertyChangedEventArgs e)
        {
            /*
            if (e.PropertyName.Equals("Position"))
            {
                UpdatePathGeometry();
                UpdateAnchorPosition();
            }
            */
        }

        internal void UpdatePathGeometry()
        {
            if (Source != null && Sink != null)
            {                
                DesignerCanvas canvas = FindAnchestor<DesignerCanvas>(source);
                if (canvas == null) return;

                List<Point> linePoints = PathFinder.GetConnectionLine(canvas, Source, Sink, true);
                if (SourceAnchorLeft != linePoints[0].Y || SourceAnchorTop != linePoints[0].X || SinkAnchorLeft != linePoints[linePoints.Count - 1].Y || SinkAnchorTop != linePoints[linePoints.Count - 1].X)
                {
                    //PathGeometry geometry = new PathGeometry();
                    SourceAnchorLeft = linePoints[0].Y;
                    SourceAnchorTop = linePoints[0].X;
                    SinkAnchorLeft = linePoints[linePoints.Count - 1].Y;
                    SinkAnchorTop = linePoints[linePoints.Count - 1].X;

                    if (linePoints.Count > 0)
                    {
                        PathFigure figure = new PathFigure();
                        figure.StartPoint = linePoints[0];

                        if (canvas.CurvedLines)
                        {

                            Vector b = linePoints[2] - linePoints[1];
                            if (b.Length < 7)
                            {
                                figure.Segments.Add(
                                    new QuadraticBezierSegment(
                                        new Point(linePoints[3].X, linePoints[0].Y),
                                        linePoints[3], true));
                            }
                            else
                            {
                                figure.Segments.Add(new QuadraticBezierSegment(linePoints[1],
                                                        new Point(linePoints[1].X + (linePoints[2].X - linePoints[1].X) * 0.5,
                                                                  linePoints[1].Y + (linePoints[2].Y - linePoints[1].Y) * 0.5),
                                                                  true));
                                figure.Segments.Add(new QuadraticBezierSegment(
                                    linePoints[2], linePoints[3], true));
                            }


                        }
                        else
                        {
                            linePoints.Remove(linePoints[0]);
                            figure.Segments.Add(new PolyLineSegment(linePoints, true));
                        }

                        (pathGeometry.Transform as TranslateTransform).X = 0;
                        (pathGeometry.Transform as TranslateTransform).Y = 0;
                        pathGeometry.Figures.Clear();
                        pathGeometry.Figures.Add(figure);


                        //Point point, tangent;
                        //geometry.GetPointAtFractionLength(0.5, out point, out tangent);
                        //geometry.AddGeometry(new EllipseGeometry(point, 5.0, 5.0));


                        //this.PathGeometry = geometry;

                        //pathGeometry = geometry;
                        //if (PropertyChanged != null) PropertyChanged.Invoke(this, new PropertyChangedEventArgs("PathGeometry"));
                        UpdateAnchorPosition();
                    }
                    else
                    {
                        (pathGeometry.Transform as TranslateTransform).X = 0;
                        (pathGeometry.Transform as TranslateTransform).Y = 0;
                        pathGeometry.Figures.Clear();
                    }
                }
            }
            else
            {
                (pathGeometry.Transform as TranslateTransform).X = 0;
                (pathGeometry.Transform as TranslateTransform).Y = 0;
                pathGeometry.Figures.Clear();
            }
        }

        private void UpdateAnchorPosition()
        {
            /*
            try
            {
                Point pathStartPoint, pathTangentAtStartPoint;
                Point pathEndPoint, pathTangentAtEndPoint;
                Point pathMidPoint, pathTangentAtMidPoint;

                if (PathGeometry == null)
                    return;

                // the PathGeometry.GetPointAtFractionLength method gets the point and a tangent vector 
                // on PathGeometry at the specified fraction of its length
                this.PathGeometry.GetPointAtFractionLength(0, out pathStartPoint, out pathTangentAtStartPoint);
                this.PathGeometry.GetPointAtFractionLength(1, out pathEndPoint, out pathTangentAtEndPoint);
                this.PathGeometry.GetPointAtFractionLength(0.5, out pathMidPoint, out pathTangentAtMidPoint);

                // get angle from tangent vector
                this.SourceAnchorAngle = Math.Atan2(-pathTangentAtStartPoint.Y, -pathTangentAtStartPoint.X) * (180 / Math.PI);
                this.SinkAnchorAngle = Math.Atan2(pathTangentAtEndPoint.Y, pathTangentAtEndPoint.X) * (180 / Math.PI);

                // add some margin on source and sink side for visual reasons only
                pathStartPoint.Offset(-pathTangentAtStartPoint.X * 5, -pathTangentAtStartPoint.Y * 5);
                pathEndPoint.Offset(pathTangentAtEndPoint.X * 5, pathTangentAtEndPoint.Y * 5);
            }
            catch (Exception ex)
            {
            }
            */
        }

        #endregion

        #region Ctor/Dtor

        static DesignerConnection()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(DesignerConnection), new FrameworkPropertyMetadata(typeof(DesignerConnection)));
            SourceAnchorTopProperty = DependencyProperty.Register("SourceAnchorTop", typeof(double), typeof(DesignerConnection),
                new FrameworkPropertyMetadata((double)0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault, new PropertyChangedCallback(OnPositioningChanged)));
            SourceAnchorLeftProperty = DependencyProperty.Register("SourceAnchorLeft", typeof(double), typeof(DesignerConnection),
                new FrameworkPropertyMetadata((double)0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault, new PropertyChangedCallback(OnPositioningChanged)));
            SinkAnchorTopProperty = DependencyProperty.Register("SinkAnchorTop", typeof(double), typeof(DesignerConnection),
                new FrameworkPropertyMetadata((double)0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault, new PropertyChangedCallback(OnPositioningChanged)));
            SinkAnchorLeftProperty = DependencyProperty.Register("SinkAnchorLeft", typeof(double), typeof(DesignerConnection),
                new FrameworkPropertyMetadata((double)0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault, new PropertyChangedCallback(OnPositioningChanged)));

            SourceAnchorAngleProperty = DependencyProperty.Register("SourceAnchorAngle", typeof(double), typeof(DesignerConnection),
                new FrameworkPropertyMetadata((double)0, FrameworkPropertyMetadataOptions.None));
            SinkAnchorAngleProperty = DependencyProperty.Register("SinkAnchorAngle", typeof(double), typeof(DesignerConnection),
                new FrameworkPropertyMetadata((double)0, FrameworkPropertyMetadataOptions.None));
        }

        public DesignerConnection()
        {
            Loaded += new RoutedEventHandler(DesignerConnection_Loaded);
        }

        #endregion

        #region INotifyPropertyChanged Members

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion

        #region Nested

        private class LeftConverter : IValueConverter
        {
            #region IValueConverter Members

            public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
            {
                return ((Connector)parameter).Position.X;
            }

            public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
            {
                throw new NotImplementedException();
            }

            #endregion
        }

        private class TopConverter : IValueConverter
        {
            #region IValueConverter Members

            public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
            {
                return ((Connector)parameter).Position.Y;
            }

            public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
            {
                throw new NotImplementedException();
            }

            #endregion
        }

        #endregion        
    }

    [Serializable()]
    public class DesignerComment : Control, IDesignerElement, System.Runtime.Serialization.ISerializable
    {
        #region Public Members

        public static readonly DependencyProperty IsSelectedProperty;
        public static readonly RoutedEvent SelectedEvent;
        public static readonly RoutedEvent UnselectedEvent;

        public bool IsSelected
        {
            get
            {
                return (bool)base.GetValue(IsSelectedProperty);
            }
            set
            {
                base.SetValue(IsSelectedProperty, value);
            }
        }

        public event RoutedEventHandler Selected
        {
            add
            {
                base.AddHandler(SelectedEvent, value);
            }
            remove
            {
                base.RemoveHandler(SelectedEvent, value);
            }
        }

        public event RoutedEventHandler Unselected
        {
            add
            {
                base.AddHandler(UnselectedEvent, value);
            }
            remove
            {
                base.RemoveHandler(UnselectedEvent, value);
            }
        }

        public string Comment
        {
            get
            {
                EditableTextBlock b = Template.FindName("Part_Comment", this) as EditableTextBlock;
                return b.Text;
            }
            set
            {
                EditableTextBlock b = Template.FindName("Part_Comment", this) as EditableTextBlock;
                if (b != null)
                {
                    b.Text = value;
                }
            }
        }

        #endregion

        #region Protected Methods

        protected virtual void OnSelected(RoutedEventArgs e)
        {
            /*
            if (ZIndex != (ParentDesigner.ZIndex - 1))
            {
                SetValue(DesignerItem.ZIndexProperty, ParentDesigner.ZIndex++);
            }
            */
            
            //this.HandleIsSelectedChanged(true, e);
        }

        protected virtual void OnUnselected(RoutedEventArgs e)
        {
            //this.HandleIsSelectedChanged(false, e);
        }

        #endregion

        #region Private Methods

        private static void OnIsSelectedChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            DesignerComment container = d as DesignerComment;
            bool newValue = (bool)e.NewValue;         
            if (newValue)
            {
                container.OnSelected(new RoutedEventArgs(Selector.SelectedEvent, container));                
            }
            else
            {
                container.OnUnselected(new RoutedEventArgs(Selector.UnselectedEvent, container));
            }
        }

        #endregion

        #region Ctor/Dtor

        static DesignerComment()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(DesignerComment),
                new FrameworkPropertyMetadata(typeof(DesignerComment)));
            IsSelectedProperty = Selector.IsSelectedProperty.AddOwner(typeof(DesignerComment),
                    new FrameworkPropertyMetadata(false, FrameworkPropertyMetadataOptions.Journal | FrameworkPropertyMetadataOptions.BindsTwoWayByDefault,
                        new PropertyChangedCallback(DesignerComment.OnIsSelectedChanged)));
            SelectedEvent = Selector.SelectedEvent.AddOwner(typeof(DesignerComment));
            UnselectedEvent = Selector.UnselectedEvent.AddOwner(typeof(DesignerComment));
        }

        public DesignerComment()
        {
            /*
            Text = "Comment";
            FontSize = 20;
            
             * */

            Loaded += new RoutedEventHandler(DesignerComment_Loaded);
        }

        void DesignerComment_Loaded(object sender, RoutedEventArgs e)
        {
            EditableTextBlock b = Template.FindName("Part_Comment", this) as EditableTextBlock;
            if (b != null)
            {
                b.OnEdit += new EventHandler(DesignerComment_OnEdit);
            }
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            if (!e.Handled)
            {
                (ItemsControl.ItemsControlFromItemContainer(this) as DesignerCanvas).NotifyDesignerItemClicked(
                    this, MouseButton.Left);
                e.Handled = true;
                //this.HandleMouseButtonDown(MouseButton.Left);
            }

            base.OnMouseLeftButtonDown(e);
        }

        protected override void OnMouseRightButtonDown(MouseButtonEventArgs e)
        {
            if (!e.Handled)
            {
                e.Handled = true;
                //this.HandleMouseButtonDown(MouseButton.Right);
            }
            base.OnMouseRightButtonDown(e);
        }


        void DesignerComment_OnEdit(object sender, EventArgs e)
        {
            EditableTextBlock edit = sender as EditableTextBlock;
            if (edit.Text == string.Empty)
            {
                if (Parent != null)
                {
                    (Parent as Canvas).Children.Remove(this);
                }
            }
        }

        #endregion

        #region ISerializable Members

        void System.Runtime.Serialization.ISerializable.GetObjectData(System.Runtime.Serialization.SerializationInfo info, System.Runtime.Serialization.StreamingContext context)
        {
            EditableTextBlock b = Template.FindName("Part_Comment", this) as EditableTextBlock;
            if (b != null)
            {
                info.AddValue("Text", b.Text);
            }
        }

        public DesignerComment(SerializationInfo info, StreamingContext context)
        {
            string value = info.GetString("Text");
            Loaded += delegate(object sender, RoutedEventArgs e)
            {
                EditableTextBlock b = Template.FindName("Part_Comment", this) as EditableTextBlock;
                if (b != null)
                {
                    b.Text = value;
                    b.OnEdit += new EventHandler(DesignerComment_OnEdit);
                }
            };
        }

       

        #endregion
    }

    public class DesignerItem : ContentControl, IDesignerElement
    {
        #region Public Members

        public static readonly DependencyProperty ZIndexProperty;
        public static readonly DependencyProperty TopProperty;
        public static readonly DependencyProperty LeftProperty;
        public static readonly DependencyProperty IsSelectedProperty;
        public static readonly RoutedEvent SelectedEvent;
        public static readonly RoutedEvent UnselectedEvent;
        public static readonly RoutedEvent PropertyChangedEvent;   


        #endregion

        #region Private Properties

        private DesignerCanvas ParentDesigner
        {
            get
            {
                return (this.ParentSelector as DesignerCanvas);
            }
        }

        internal Selector ParentSelector
        {
            get
            {
                return (ItemsControl.ItemsControlFromItemContainer(this) as Selector);
            }
        }

        #endregion

        #region Public Properties

        public int ZIndex
        {
            get { return (int)GetValue(DesignerItem.ZIndexProperty); }
            set { SetValue(DesignerItem.ZIndexProperty, value); }
        }

        public double Top
        {
            get { return (double)GetValue(DesignerItem.TopProperty); }
            set { SetValue(DesignerItem.TopProperty, value); }
        }

        public double Left
        {
            get { return (double)GetValue(DesignerItem.LeftProperty); }
            set { SetValue(DesignerItem.LeftProperty, value); }
        }

        public bool IsSelected
        {
            get
            {
                return (bool)base.GetValue(IsSelectedProperty);
            }
            set
            {
                base.SetValue(IsSelectedProperty, value);
            }
        }

        public event RoutedEventHandler Selected
        {
            add
            {
                base.AddHandler(SelectedEvent, value);
            }
            remove
            {
                base.RemoveHandler(SelectedEvent, value);
            }
        }

        public event RoutedEventHandler Unselected
        {
            add
            {
                base.AddHandler(UnselectedEvent, value);
            }
            remove
            {
                base.RemoveHandler(UnselectedEvent, value);
            }
        }


        #endregion        

        #region Private Methods

        protected virtual void OnSelected(RoutedEventArgs e)
        {           
            if (ZIndex != (ParentDesigner.ZIndex - 1))
            {
                SetValue(DesignerItem.ZIndexProperty, ParentDesigner.ZIndex++);
            }

            this.HandleIsSelectedChanged(true, e);
        }

        protected virtual void OnUnselected(RoutedEventArgs e)
        {
            this.HandleIsSelectedChanged(false, e);
        }

        protected override void OnMouseDoubleClick(MouseButtonEventArgs e)
        {
            if (!e.Handled)
            {
                e.Handled = true;

                DesignerCanvas parentDesigner = this.ParentDesigner;
                if (parentDesigner != null)
                {
                    parentDesigner.NotifyDesignerItemDoubleClicked(this);
                }
            }
            base.OnMouseDoubleClick(e);
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            if (!e.Handled)
            {
                e.Handled = true;
                this.HandleMouseButtonDown(MouseButton.Left);
            }
            base.OnMouseLeftButtonDown(e);
        }

        protected override void OnMouseRightButtonDown(MouseButtonEventArgs e)
        {
            if (!e.Handled)
            {
                e.Handled = true;
                this.HandleMouseButtonDown(MouseButton.Right);
            }
            base.OnMouseRightButtonDown(e);
        }

        private static void OnIsSelectedChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            DesignerItem container = d as DesignerItem;
            bool newValue = (bool)e.NewValue;
            Selector parentSelector = container.ParentSelector;

            if (newValue)
            {
                container.OnSelected(new RoutedEventArgs(Selector.SelectedEvent, container));
            }
            else
            {
                container.OnUnselected(new RoutedEventArgs(Selector.UnselectedEvent, container));
            }
        }

        private void HandleIsSelectedChanged(bool newValue, RoutedEventArgs e)
        {
            base.RaiseEvent(e);
        }

        private void HandleMouseButtonDown(MouseButton mouseButton)
        {
            if (DesignerCanvas.UiGetIsSelectable(this) && base.Focus())
            {
                DesignerCanvas parentDesigner = this.ParentDesigner;
                if (parentDesigner != null)
                {
                    parentDesigner.NotifyDesignerItemClicked(this, mouseButton);
                }
            }
        }

        #endregion

        #region Public Methods

        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();
        }

        #endregion

        #region Ctor/Dtor

        static DesignerItem()
        {
            try
            {
                DefaultStyleKeyProperty.OverrideMetadata(typeof(DesignerItem), new FrameworkPropertyMetadata(typeof(DesignerItem)));
                IsSelectedProperty = Selector.IsSelectedProperty.AddOwner(typeof(DesignerItem),
                    new FrameworkPropertyMetadata(false, FrameworkPropertyMetadataOptions.Journal | FrameworkPropertyMetadataOptions.BindsTwoWayByDefault,
                        new PropertyChangedCallback(DesignerItem.OnIsSelectedChanged)));
                SelectedEvent = Selector.SelectedEvent.AddOwner(typeof(DesignerItem));
                UnselectedEvent = Selector.UnselectedEvent.AddOwner(typeof(DesignerItem));
                ZIndexProperty = DependencyProperty.Register("ZIndex", typeof(int), typeof(DesignerItem),
                    new FrameworkPropertyMetadata((int)0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
                TopProperty = DependencyProperty.Register("Top", typeof(double), typeof(DesignerItem),
                    new FrameworkPropertyMetadata((double)0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
                LeftProperty = DependencyProperty.Register("Left", typeof(double), typeof(DesignerItem),
                    new FrameworkPropertyMetadata((double)0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
                PropertyChangedEvent = EventManager.RegisterRoutedEvent("PropertyChanged", RoutingStrategy.Bubble, 
                    typeof(RoutedEventHandler), typeof(DesignerItem));

            }
            catch (Exception ex)
            {

            }
        }

        public DesignerItem()
        {
            DataContextChanged += new DependencyPropertyChangedEventHandler(DesignerItem_DataContextChanged);
        }

        void DesignerItem_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (e.OldValue is INotifyPropertyChanged)
                (e.OldValue as INotifyPropertyChanged).PropertyChanged -= DesignerItem_PropertyChanged;
            if( e.NewValue is INotifyPropertyChanged )
                (e.NewValue as INotifyPropertyChanged).PropertyChanged += DesignerItem_PropertyChanged;            
        }

        void DesignerItem_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            RaiseEvent(new RoutedEventArgs(PropertyChangedEvent));
        }

        #endregion

    }


    public enum ConnectorGroups
    {
        Source,
        Sink,
    };

    public class Connector : Control, INotifyPropertyChanged
    {
        #region Private Members

        private DesignerItem parentDesignerItem;
        // drag start point, relative to the DesignerCanvas
        private Point? dragStartPoint = null;
        // center position of this Connector relative to the DesignerCanvas
        private Point position;    
        // connector groups
        private ConnectorGroups group;


        #endregion

        #region Public Members

        public static readonly DependencyProperty NormalProperty = DependencyProperty.RegisterAttached("Normal", typeof(Vector?), typeof(Connector));

        public Point Position
        {
            get { return position; }
            set
            {
                if (position != value)
                {
                    position = value;
                    if (PropertyChanged != null)
                        PropertyChanged.Invoke(this, new PropertyChangedEventArgs("Position"));
                }
            }
        }

        public Vector? Normal
        {
            get { return GetValue(NormalProperty) as Vector?; }
            set
            {
                SetValue(NormalProperty, value);
            }
        }

        public DesignerItem ParentDesignerItem
        {
            get
            {
                if (parentDesignerItem == null)
                    parentDesignerItem = FindAnchestor<DesignerItem>(this);

                return parentDesignerItem;
            }
        }

        public ConnectorGroups Group
        {
            get
            {
                return group;
            }
            set
            {
                group = value;
            }
        }

        #endregion

        #region Private Methods

        // when the layout changes we update the position property
        void Connector_LayoutUpdated(object sender, EventArgs e)
        {            
            ScrollViewer viewer = FindAnchestor<ScrollViewer>(this);
            if (viewer != null)
            {
                DesignerCanvas designer = GetDesignerCanvas(this);
                if (designer != null)
                {
                     //get centre position of this Connector relative to the DesignerCanvas
                    //double invZoom = 1.0f / designer.Zoom;
                    //Point a = this.TransformToAncestor(viewer).Transform(new Point(this.Width / 2, this.Height / 2));                  
                    Point a = designer.Transform(this, new Point(this.Width / 2, this.Height / 2));
                    /*
                    a += new Vector(-(double)viewer.GetValue(ScrollViewer.HorizontalOffsetProperty),
                        -(double)viewer.GetValue(ScrollViewer.VerticalOffsetProperty));
                    */
                    Point b = this.Position;

                    double LengthSquared = (a - b).LengthSquared;
                    if (LengthSquared > (0.5f * 0.5f) )
                    {
                        this.Position = a;
                    }                 
                }
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

        // iterate through visual tree to get parent DesignerCanvas
        private DesignerCanvas GetDesignerCanvas(DependencyObject element)
        {
            while (element != null && !(element is DesignerCanvas))
                element = VisualTreeHelper.GetParent(element);
            return element as DesignerCanvas;
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            DesignerCanvas canvas = GetDesignerCanvas(this);
            if (canvas != null)
            {
                // position relative to DesignerCanvas
                this.dragStartPoint = new Point?(e.GetPosition(canvas));
                e.Handled = true;
            }
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            // if mouse button is not pressed we have no drag operation, ...
            if (e.LeftButton != MouseButtonState.Pressed)
                this.dragStartPoint = null;

            // but if mouse button is pressed and start point value is set we do have one
            if (this.dragStartPoint.HasValue)
            {
                // create connection adorner 
                DesignerCanvas canvas = GetDesignerCanvas(this);
                if (canvas != null)
                {
                    AdornerLayer adornerLayer = AdornerLayer.GetAdornerLayer(canvas);
                    if (adornerLayer != null)
                    {
                        ConnectorAdorner adorner = new ConnectorAdorner(canvas, this);
                        if (adorner != null)
                        {
                            adornerLayer.Add(adorner);
                            e.Handled = true;
                        }
                    }
                }
            }
        }


        protected override Size ArrangeOverride(Size arrangeBounds)
        {
            Size size = base.ArrangeOverride(arrangeBounds);
            return size;
        }

        #endregion

        #region Ctor/Dtor

        static Connector()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(Connector), new FrameworkPropertyMetadata(typeof(Connector)));
        }

        public Connector()
        {
            // fired when layout changes
            base.LayoutUpdated += new EventHandler(Connector_LayoutUpdated);                        
        }

        #endregion

        #region INotifyPropertyChanged Members

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion
    }

    public class MoveThumb : Thumb
    {
        #region Private Methods


        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {

            //FindAnchestor<IDesignerElement>(this)
           
            if (!e.Handled)
            {
                IDesignerElement element = FindDesignerElement(this);
                if (element != null)
                {
                    DesignerCanvas canvas = ItemsControl.ItemsControlFromItemContainer(element as DependencyObject) as DesignerCanvas;
                    if (canvas != null)
                    {
                        if (DesignerCanvas.UiGetIsSelectable(element as DependencyObject) && (element as UIElement).Focus())
                        {
                            canvas.NotifyDesignerItemClicked(element, MouseButton.Left);
                            if (canvas.SelectedItems.Contains(canvas.ItemContainerGenerator.ItemFromContainer(element as DependencyObject)))
                            {
                                base.OnMouseLeftButtonDown(e);
                                e.Handled = true;
                            }
                            else
                            {
                                e.Handled = true;
                            }
                        }                                                
                    }
                }
                
                //this.HandleMouseButtonDown(MouseButton.Left);
            }                        
        }

        void MoveThumb_DragCompleted(object sender, DragCompletedEventArgs e)
        {
            System.Collections.Generic.HashSet<DesignerItem> items = new HashSet<DesignerItem>();
            DesignerCanvas canvas = FindAnchestor<DesignerCanvas>(this);
            canvas.NotifyDragCompleted();

            foreach (object o in canvas.SelectedItems)
            {
                UIElement item2 = canvas.ItemContainerGenerator.ContainerFromItem(o) as UIElement;
                TranslateTransform t = (item2.RenderTransform as TranslateTransform);
                if (t == null) item2.RenderTransform = t = new TranslateTransform();                
                double left = ResizeableCanvas.GetLeft(item2);
                double top = ResizeableCanvas.GetTop(item2);
                ResizeableCanvas.SetLeft(item2, left + t.X);
                ResizeableCanvas.SetTop(item2, top + t.Y);
                t.X = 0; t.Y = 0;
                if (item2 is DesignerItem)
                    items.Add(item2 as DesignerItem);
            }

            foreach (DesignerConnection connection in canvas.Connections)
            {
                connection.suspendlayout = 0;
            }
        }

        void MoveThumb_DragStarted(object sender, DragStartedEventArgs e)
        {
            System.Collections.Generic.HashSet<DesignerItem> items = new HashSet<DesignerItem>();
            DesignerCanvas canvas = FindAnchestor<DesignerCanvas>(this);
            canvas.NotifyDragStarted();

            foreach (object o in canvas.SelectedItems)
            {
                UIElement item2 = canvas.ItemContainerGenerator.ContainerFromItem(o) as UIElement;
                TranslateTransform t = (item2.RenderTransform as TranslateTransform);
                if (t == null) item2.RenderTransform = t = new TranslateTransform();
                if (item2 is DesignerItem)
                    items.Add(item2 as DesignerItem);
            }

            foreach (DesignerConnection connection in canvas.Connections)
            {
                if (connection.Source != null && connection.Sink != null)
                {
                    try
                    {
                        bool c1 = items.Contains(connection.Source.ParentDesignerItem);
                        bool c2 = items.Contains(connection.Sink.ParentDesignerItem);
                        if (c1 && c2)
                        {
                            if (connection.Source.DataContext is GraphProperty)
                                Console.WriteLine("E: {0} {1}", (connection.Source.DataContext as GraphProperty).Name, (connection.Sink.DataContext as GraphProperty).Name);
                            else
                                Console.WriteLine("E: {0} {1}", (connection.Source.DataContext as GraphSocket).Name, (connection.Sink.DataContext as GraphSocket).Name);

                            connection.suspendlayout = 1;
                        }
                        else if (c1 || c2)
                        {
                            if (connection.Source.DataContext is GraphProperty)
                                Console.WriteLine("S: {0} {1}", (connection.Source.DataContext as GraphProperty).Name, (connection.Sink.DataContext as GraphProperty).Name);
                            else
                                Console.WriteLine("S: {0} {1}", (connection.Source.DataContext as GraphSocket).Name, (connection.Sink.DataContext as GraphSocket).Name);
                            connection.suspendlayout = 2;
                        }
                        else
                        {
                            if (connection.Source.DataContext is GraphProperty)
                                Console.WriteLine("D: {0} {1}", (connection.Source.DataContext as GraphProperty).Name, (connection.Sink.DataContext as GraphProperty).Name);
                            else
                                Console.WriteLine("D: {0} {1}", (connection.Source.DataContext as GraphSocket).Name, (connection.Sink.DataContext as GraphSocket).Name);
                            connection.suspendlayout = 0;
                        }
                    }
                    catch (Exception ex)
                    {
                    }
                }                
            }
        }

        private void MoveThumb_DragDelta(object sender, DragDeltaEventArgs e)
        {
            System.Collections.Generic.HashSet<DesignerItem> items = new HashSet<DesignerItem>();
            DesignerCanvas canvas = FindAnchestor<DesignerCanvas>(this);
            IDesignerElement item = this.DataContext as IDesignerElement;
            if (item != null)
            {
                foreach (object o in canvas.SelectedItems)
                {
                    UIElement item2 = canvas.ItemContainerGenerator.ContainerFromItem(o) as UIElement;
                    TranslateTransform t = (item2.RenderTransform as TranslateTransform);
                    if (t == null) item2.RenderTransform = t = new TranslateTransform();
                    t.X += e.HorizontalChange;
                    t.Y += e.VerticalChange;

                    if (item2 is DesignerItem)
                        items.Add(item2 as DesignerItem);
                    
                }
            }

            foreach (DesignerConnection connection in canvas.Connections)
            {               
                if (connection.suspendlayout == 1)
                {
                    (connection.PathGeometry.Transform as TranslateTransform).X += e.HorizontalChange;
                    (connection.PathGeometry.Transform as TranslateTransform).Y += e.VerticalChange;
                }
                else if (connection.suspendlayout == 2)
                {
                    connection.UpdatePathGeometry();
                }

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

        private static IDesignerElement FindDesignerElement(DependencyObject current)
        {
            do
            {
                if (current is IDesignerElement)
                {
                    return (IDesignerElement)current;
                }
                current = VisualTreeHelper.GetParent(current);
            }
            while (current != null);
            return null;
        }

        #endregion

        #region Ctor / Dtor

        static MoveThumb()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(MoveThumb), new FrameworkPropertyMetadata(typeof(MoveThumb)));
        }

        public MoveThumb()
        {
            DragDelta += new DragDeltaEventHandler(this.MoveThumb_DragDelta);
            DragStarted += new DragStartedEventHandler(MoveThumb_DragStarted);
            DragCompleted += new DragCompletedEventHandler(MoveThumb_DragCompleted);

        }



        #endregion
    }

    public class PathFinder
    {
        public static List<Point> GetConnectionLine(DesignerCanvas canvas, Connector Source, Connector Sink, bool something)
        {
            //double invZoom = 1.0 / canvas.Zoom;

            double sx = Source.ParentDesignerItem.RenderTransform.Value.OffsetX;
            double sy = Source.ParentDesignerItem.RenderTransform.Value.OffsetY;
            double tx = Sink.ParentDesignerItem.RenderTransform.Value.OffsetX;
            double ty = Sink.ParentDesignerItem.RenderTransform.Value.OffsetY;
            double invZoom = 1.0;
            List<Point> points = new List<Point>();            
            Point a = new Point((Source.Position.X + sx) * invZoom, (Source.Position.Y + sy) * invZoom);
            Point b = new Point((Sink.Position.X + tx) * invZoom, (Sink.Position.Y + ty) * invZoom);            

            /*
            Point a = new Point(Source.ParentDesignerItem.Left, Source.ParentDesignerItem.Top);
            Point b = new Point(Sink.ParentDesignerItem.Left, Sink.ParentDesignerItem.Top);
             * */


            points.Add(a);
            if (Source.Normal.HasValue)
            {
                points.Add(a + Source.Normal.Value);
            }

            if (Sink.Normal.HasValue)
            {
                points.Add(b + Sink.Normal.Value);
            }
            points.Add(b);
            return points;
        }
    }

    public class DesignerConnectionAdorner : Adorner
    {
        #region Private Members

        private readonly VisualCollection _collection;
        private readonly Canvas _canvas;

        #endregion

        #region Private Methods

        protected override Visual GetVisualChild(int index)
        {
            return _collection[index];
        }

        protected override int VisualChildrenCount
        {
            get
            {
                return _collection.Count;
            }
        }

        protected override Size ArrangeOverride(Size finalSize)
        {
            _canvas.Arrange(new Rect(0, 0, _canvas.ActualWidth, _canvas.ActualHeight));
            return finalSize;
        }

        protected override void OnRender(DrawingContext drawingContext)
        {

        }

        #endregion

        #region Ctor / Dtor

        public DesignerConnectionAdorner(UIElement boundElement)
            : base(boundElement)
        {
            _collection = new VisualCollection(this);
            _canvas = new Canvas();
            _canvas.Background = Brushes.Red;
            _collection.Add(_canvas);
        }

        #endregion
    }

    public class ConnectorAdorner : Adorner
    {
        #region Private Members

        private DesignerCanvas designerCanvas;
        private Connector sourceConnector;
        private Pen drawingPen;

        private DesignerItem hitDesignerItem;
        private DesignerItem HitDesignerItem
        {
            get { return hitDesignerItem; }
            set
            {
                if (hitDesignerItem != value)
                {

                    /*
                    if (hitDesignerItem != null)
                        hitDesignerItem.IsDragConnectionOver = false;
                    */


                    hitDesignerItem = value;

                    /*
                    if (hitDesignerItem != null)
                        hitDesignerItem.IsDragConnectionOver = true;
                     */
                }
            }
        }

        private Connector hitConnector;
        private Connector HitConnector
        {
            get { return hitConnector; }
            set
            {
                if (hitConnector != value)
                {
                    hitConnector = value;
                }
            }
        }

        #endregion

        #region Ctor / Dtor

        public ConnectorAdorner(DesignerCanvas designer, Connector sourceConnector)
            : base(designer)
        {
            this.designerCanvas = designer;
            this.sourceConnector = sourceConnector;
            drawingPen = new Pen(Brushes.LightSlateGray, 1);
            drawingPen.LineJoin = PenLineJoin.Round;
            this.Cursor = Cursors.Cross;
        }

        #endregion

        #region Private Methods

        protected override void OnMouseUp(MouseButtonEventArgs e)
        {
            if (HitConnector != null)
            {
                /*
                
                Connection newConnection = new Connection(sourceConnector, sinkConnector);
                */

                Connector sourceConnector = this.sourceConnector;
                Connector sinkConnector = this.HitConnector;
                //this.sourceConnector.ParentDesignerItem;


                int count = (from DesignerConnection c in designerCanvas.Connections
                             where (c.Source == sourceConnector && c.Sink == sinkConnector) ||
                                   (c.Source == sinkConnector && c.Sink == sourceConnector)
                             select c).Count();
                if (count == 0)
                {
                    if (designerCanvas.IsLinkable(sourceConnector, sinkConnector))
                    {
                        /*
                        canvas.Children.Add(new DesignerConnection()
                        {
                            Source = sourceConnector,
                            Sink = sinkConnector
                        });
                        */

                        if (/*sinkConnector.Name == "Part_Source"*/ sinkConnector.Group == ConnectorGroups.Sink)
                        {
                            designerCanvas.MyConnections.Add(new DesignerConnection()
                            {
                                Source = sinkConnector,
                                Sink = sourceConnector
                            });
                        }
                        else
                        {
                            designerCanvas.MyConnections.Add(new DesignerConnection()
                            {
                                Source = sourceConnector,
                                Sink = sinkConnector
                            });
                        }

                        
                        designerCanvas.NotifyConnectionLink(sourceConnector, sinkConnector);
                    }
                }
                else if (count == 1 && Keyboard.IsKeyDown(Key.LeftCtrl))
                {
                    var array = (from DesignerConnection c in designerCanvas.Connections
                                 where (c.Source == sourceConnector && c.Sink == sinkConnector) ||
                                       (c.Source == sinkConnector && c.Sink == sourceConnector)
                                 select c).ToArray();
                    foreach (DesignerConnection connection in array)
                    {
                        //canvas.Children.Remove(connection);
                        designerCanvas.MyConnections.Remove(connection);
                        designerCanvas.NotifyConnectionUnlink(connection.Source, connection.Sink);
                    }
                }
            }
            if (HitDesignerItem != null)
            {
                //this.HitDesignerItem.IsDragConnectionOver = false;
            }

            if (this.IsMouseCaptured) this.ReleaseMouseCapture();

            AdornerLayer adornerLayer = AdornerLayer.GetAdornerLayer(this.designerCanvas);
            if (adornerLayer != null)
            {
                adornerLayer.Remove(this);
            }
        }
        protected override void OnMouseMove(MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                if (!this.IsMouseCaptured) this.CaptureMouse();
                HitTesting(e.GetPosition(this));
                this.InvalidateVisual();
            }
            else
            {
                if (this.IsMouseCaptured) this.ReleaseMouseCapture();
            }
        }
        protected override void OnRender(DrawingContext dc)
        {
            base.OnRender(dc);

            // without a background the OnMouseMove event would not be fired
            // Alternative: implement a Canvas as a child of this adorner, like
            // the ConnectionAdorner does.
            dc.DrawRectangle(Brushes.Transparent, null, new Rect(RenderSize));
        }
        private void HitTesting(Point hitPoint)
        {
            bool hitConnectorFlag = false;

            Cursor = Cursors.Cross;
            DependencyObject hitObject = designerCanvas.InputHitTest(hitPoint) as DependencyObject;
            while (hitObject != null &&
                   hitObject != sourceConnector.ParentDesignerItem &&
                   hitObject.GetType() != typeof(DesignerCanvas))
            {
                if (hitObject is Connector)
                {
                    HitConnector = hitObject as Connector;
                    hitConnectorFlag = true;

                    if (!designerCanvas.IsLinkable(sourceConnector, hitObject as Connector))
                    {
                        Cursor = Cursors.No;
                    }
                }

                if (hitObject is DesignerItem)
                {
                    HitDesignerItem = hitObject as DesignerItem;
                    if (!hitConnectorFlag)
                        HitConnector = null;
                    return;
                }
                hitObject = VisualTreeHelper.GetParent(hitObject);
            }

            HitConnector = null;
            HitDesignerItem = null;
        }

        #endregion

    }

    public class SelectionAdorner : Adorner
    {
        #region Private Members

        private DesignerCanvas designerCanvas;
        private Point startPoint;
        private Pen drawingPen;
        private Brush bg;

        #endregion

        #region Ctor/Dtor

        public SelectionAdorner(DesignerCanvas designer)
            : base(designer)
        {
            this.designerCanvas = designer;
            startPoint = Mouse.GetPosition(designer);
            drawingPen = new Pen(Brushes.LightSlateGray, 0.5f);
            drawingPen.LineJoin = PenLineJoin.Round;
            bg = new SolidColorBrush(Colors.LightSlateGray);
            bg.Opacity = 0.1f;
            Cursor = Cursors.Cross;
            Mouse.Capture(this, CaptureMode.Element);
        }

        #endregion

        #region Private Methods

        protected override void OnMouseMove(MouseEventArgs e)
        {
            InvalidateVisual();
            base.OnMouseMove(e);
        }

        protected override void OnMouseRightButtonUp(MouseButtonEventArgs e)
        {
            e.Handled = true;
            Point endPoint = Mouse.GetPosition(designerCanvas);
            Mouse.Capture(null, CaptureMode.None);

            //designerCanvas

            AdornerLayer adornerLayer = AdornerLayer.GetAdornerLayer(designerCanvas);
            if (adornerLayer != null)
            {
                adornerLayer.Remove(this);
            }


            if (Keyboard.Modifiers == ModifierKeys.None || Keyboard.Modifiers == ModifierKeys.Alt || Keyboard.Modifiers == ModifierKeys.Windows)
                designerCanvas.SelectedItems.Clear();

            Rect rect = new Rect(
                designerCanvas.TranslatePoint(startPoint,
                    designerCanvas.Template.FindName("Items", designerCanvas) as ItemsPresenter),
                designerCanvas.TranslatePoint(endPoint,
                    designerCanvas.Template.FindName("Items", designerCanvas) as ItemsPresenter)
            );            

            for (int i = 0; i < designerCanvas.Items.Count; i++)
            {
                UIElement obj = designerCanvas.ItemContainerGenerator.ContainerFromIndex(i) as UIElement;
                if (obj != null)
                {
                    Rect rect2 = VisualTreeHelper.GetDescendantBounds(obj);
                    rect2.Offset(VisualTreeHelper.GetOffset(obj));
                    if (rect.IntersectsWith(rect2))
                    {                           
                        if (Keyboard.Modifiers == ModifierKeys.Control)
                            designerCanvas.SelectedItems.Remove(designerCanvas.Items[i]);
                        else 
                            designerCanvas.SelectedItems.Add(designerCanvas.Items[i]);
                    }
                }
            }
        }

        protected override void OnRender(DrawingContext dc)
        {
            base.OnRender(dc);
            Point endPoint = Mouse.GetPosition(designerCanvas);
            dc.DrawRectangle(Brushes.Transparent, null, new Rect(RenderSize));
            dc.DrawRectangle(bg, drawingPen, new Rect(startPoint, endPoint));
        }

        #endregion
    }

    public class BooleanToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter,
                              CultureInfo culture)
        {
            if (value is Boolean)
            {
                return ((bool)value) ? Visibility.Visible : Visibility.Collapsed;
            }

            return value;
        }

        public object ConvertBack(object value, Type targetType, object parameter,
                                  CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

}
