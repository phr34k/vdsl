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

namespace Aurora.SandboxDesigner.Controls
{

    public class Vector1 : Control, System.ComponentModel.INotifyPropertyChanged, IInputElement
    {
        public static readonly DependencyProperty TextProperty;
        float componentx;

        static Vector1()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(Vector1), new FrameworkPropertyMetadata(typeof(Vector1)));
            TextProperty = DependencyProperty.Register("Text", typeof(string), typeof(Vector1),
                new FrameworkPropertyMetadata(string.Empty, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault, TextChanged));
        }

        public string Text
        {
            get
            {
                return GetValue(TextProperty) as string;
            }
            set
            {
                if (string.IsNullOrEmpty(value))
                    SetValue(TextProperty, string.Empty);
                else
                    SetValue(TextProperty, value);
            }
        }

        public float ComponentX
        {
            get
            {
                return componentx;
            }
            set
            {
                if (componentx != value)
                {
                    componentx = value;
                    if (PropertyChanged != null)
                        PropertyChanged.Invoke(this, new System.ComponentModel.PropertyChangedEventArgs("ComponentX"));
                    ComponentChanged();
                }
            }
        }

        private void ComponentChanged()
        {
            string o = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0}", componentx);
            if (Text.Trim() != o)
            {
                Text = o;
            }
        }

        private static void TextChanged(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            Vector1 textBlock = obj as Vector1;
            if (null != textBlock)
            {
                string[] v = textBlock.Text.Split(new char[] { ' ' });
                if (v.Length == 1)
                {
                    float a = 0.0f;
                    float.TryParse(v[0], System.Globalization.NumberStyles.Float, System.Globalization.CultureInfo.InvariantCulture, out a);
                    textBlock.componentx = a;
                    if (textBlock.PropertyChanged != null)
                        textBlock.PropertyChanged.Invoke(textBlock, new System.ComponentModel.PropertyChangedEventArgs("ComponentX"));
                }
            }
        }

        protected override void OnMouseDown(MouseButtonEventArgs e)
        {
            Focus();
            base.OnMouseDown(e);
            e.Handled = true;
        }


        #region INotifyPropertyChanged Members

        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;

        #endregion
    }

    public class Vector2 : Control, System.ComponentModel.INotifyPropertyChanged, IInputElement
    {
        public static readonly DependencyProperty TextProperty;
        float componentx, componenty;

        static Vector2()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(Vector2), new FrameworkPropertyMetadata(typeof(Vector2)));
            TextProperty = DependencyProperty.Register("Text", typeof(string), typeof(Vector2),
                new FrameworkPropertyMetadata(string.Empty, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault, TextChanged));
        }

        public string Text
        {
            get
            {
                return GetValue(TextProperty) as string;
            }
            set
            {
                if (string.IsNullOrEmpty(value))
                    SetValue(TextProperty, string.Empty);
                else
                    SetValue(TextProperty, value);
            }
        }

        public float ComponentX
        {
            get
            {
                return componentx;
            }
            set
            {
                if (componentx != value)
                {
                    componentx = value;
                    if (PropertyChanged != null)
                        PropertyChanged.Invoke(this, new System.ComponentModel.PropertyChangedEventArgs("ComponentX"));
                    ComponentChanged();
                }
            }
        }

        public float ComponentY
        {
            get
            {
                return componenty;
            }
            set
            {
                if (componenty != value)
                {
                    componenty = value;
                    if (PropertyChanged != null)
                        PropertyChanged.Invoke(this, new System.ComponentModel.PropertyChangedEventArgs("ComponentY"));
                    ComponentChanged();
                }
            }
        }

        private void ComponentChanged()
        {
            string o = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0} {1}", componentx, componenty);
            if (Text.Trim() != o)
            {
                Text = o;
            }
        }

        private static void TextChanged(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            Vector2 textBlock = obj as Vector2;
            if (null != textBlock)
            {
                string[] v = textBlock.Text.Split(new char[] { ' ' });
                if (v.Length == 2)
                {
                    float a = float.Parse(v[0], System.Globalization.CultureInfo.InvariantCulture);
                    float b = float.Parse(v[1], System.Globalization.CultureInfo.InvariantCulture);
                    textBlock.componentx = a;
                    textBlock.componenty = b;

                    if (textBlock.PropertyChanged != null)
                        textBlock.PropertyChanged.Invoke(textBlock, new System.ComponentModel.PropertyChangedEventArgs("ComponentX"));
                    if (textBlock.PropertyChanged != null)
                        textBlock.PropertyChanged.Invoke(textBlock, new System.ComponentModel.PropertyChangedEventArgs("ComponentY"));
                }
            }
        }

        protected override void OnMouseDown(MouseButtonEventArgs e)
        {
            Focus();
            base.OnMouseDown(e);
            e.Handled = true;
        }


        #region INotifyPropertyChanged Members

        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;

        #endregion
    }

    public class Vector3 : Control, System.ComponentModel.INotifyPropertyChanged, IInputElement
    {
        public static readonly DependencyProperty TextProperty;
        float componentx, componenty, componentz;

        static Vector3()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(Vector3), new FrameworkPropertyMetadata(typeof(Vector3)));
            TextProperty = DependencyProperty.Register("Text", typeof(string), typeof(Vector3),
                new FrameworkPropertyMetadata(string.Empty, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault, TextChanged));            
        }

        public string Text
        {
            get
            {                
                return GetValue(TextProperty) as string;
            }
            set
            {
                if (string.IsNullOrEmpty(value))
                    SetValue(TextProperty, string.Empty);
                else
                    SetValue(TextProperty, value);
            }
        }

        public float ComponentX
        {
            get
            {
                return componentx;
            }
            set
            {
                if (componentx != value)
                {
                    componentx = value;
                    if( PropertyChanged != null )
                        PropertyChanged.Invoke(this, new System.ComponentModel.PropertyChangedEventArgs("ComponentX"));
                    ComponentChanged();
                }
            }
        }

        public float ComponentY
        {
            get
            {
                return componenty;
            }
            set
            {
                if (componenty != value)
                {
                    componenty = value;
                    if( PropertyChanged != null )
                        PropertyChanged.Invoke(this, new System.ComponentModel.PropertyChangedEventArgs("ComponentY"));
                    ComponentChanged();
                }
            }
        }

        public float ComponentZ
        {
            get
            {
                return componentz;
            }
            set
            {
                if (componentz != value)
                {
                    componentz = value;
                    if( PropertyChanged != null )
                        PropertyChanged.Invoke(this, new System.ComponentModel.PropertyChangedEventArgs("ComponentZ"));
                    ComponentChanged();
                }
            }
        }

        private void ComponentChanged()
        {
            string o = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0} {1} {2}", componentx, componenty, componentz);
            if (Text.Trim() != o)
            {
                Text = o;
            }
        }

        private static void TextChanged(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            Vector3 textBlock = obj as Vector3;
            if (null != textBlock)
            {
                string[] v = textBlock.Text.Split(new char[] { ' ' });
                if (v.Length == 3)
                {
                    float a = float.Parse(v[0], System.Globalization.CultureInfo.InvariantCulture);
                    float b = float.Parse(v[1], System.Globalization.CultureInfo.InvariantCulture);
                    float c = float.Parse(v[2], System.Globalization.CultureInfo.InvariantCulture);
                    textBlock.componentx = a;
                    textBlock.componenty = b;
                    textBlock.componentz = c;

                    if( textBlock.PropertyChanged != null )
                        textBlock.PropertyChanged.Invoke(textBlock, new System.ComponentModel.PropertyChangedEventArgs("ComponentX"));
                    if( textBlock.PropertyChanged != null )
                        textBlock.PropertyChanged.Invoke(textBlock, new System.ComponentModel.PropertyChangedEventArgs("ComponentY"));
                    if( textBlock.PropertyChanged != null )
                        textBlock.PropertyChanged.Invoke(textBlock, new System.ComponentModel.PropertyChangedEventArgs("ComponentZ"));
                }                               
            }
        }

        protected override void OnMouseDown(MouseButtonEventArgs e)
        {
            Focus();
            base.OnMouseDown(e);
            e.Handled = true;
        }


        #region INotifyPropertyChanged Members

        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;

        #endregion
    }

    public class Vector4 : Control, System.ComponentModel.INotifyPropertyChanged, IInputElement
    {
        public static readonly DependencyProperty TextProperty;
        float componentx, componenty, componentz, componentw;

        static Vector4()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(Vector4), new FrameworkPropertyMetadata(typeof(Vector4)));
            TextProperty = DependencyProperty.Register("Text", typeof(string), typeof(Vector4),
                new FrameworkPropertyMetadata(string.Empty, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault, TextChanged));
        }

        public string Text
        {
            get
            {
                return GetValue(TextProperty) as string;
            }
            set
            {
                if (string.IsNullOrEmpty(value))
                    SetValue(TextProperty, string.Empty);
                else
                    SetValue(TextProperty, value);
            }
        }

        public float ComponentX
        {
            get
            {
                return componentx;
            }
            set
            {
                if (componentx != value)
                {
                    componentx = value;
                    if (PropertyChanged != null)
                        PropertyChanged.Invoke(this, new System.ComponentModel.PropertyChangedEventArgs("ComponentX"));
                    ComponentChanged();
                }
            }
        }

        public float ComponentY
        {
            get
            {
                return componenty;
            }
            set
            {
                if (componenty != value)
                {
                    componenty = value;
                    if (PropertyChanged != null)
                        PropertyChanged.Invoke(this, new System.ComponentModel.PropertyChangedEventArgs("ComponentY"));
                    ComponentChanged();
                }
            }
        }

        public float ComponentZ
        {
            get
            {
                return componentz;
            }
            set
            {
                if (componentz != value)
                {
                    componentz = value;
                    if (PropertyChanged != null)
                        PropertyChanged.Invoke(this, new System.ComponentModel.PropertyChangedEventArgs("ComponentZ"));
                    ComponentChanged();
                }
            }
        }

        public float ComponentW
        {
            get
            {
                return componentw;
            }
            set
            {
                if (componentw != value)
                {
                    componentw = value;
                    if (PropertyChanged != null)
                        PropertyChanged.Invoke(this, new System.ComponentModel.PropertyChangedEventArgs("ComponentW"));
                    ComponentChanged();
                }
            }
        }

        private void ComponentChanged()
        {
            string o = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0} {1} {2} {3}", componentx, componenty, componentz, componentw);
            if (Text.Trim() != o)
            {
                Text = o;
            }
        }

        private static void TextChanged(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            Vector4 textBlock = obj as Vector4;
            if (null != textBlock)
            {
                string[] v = textBlock.Text.Split(new char[] { ' ' });
                if (v.Length == 4)
                {
                    float a = float.Parse(v[0], System.Globalization.CultureInfo.InvariantCulture);
                    float b = float.Parse(v[1], System.Globalization.CultureInfo.InvariantCulture);
                    float c = float.Parse(v[2], System.Globalization.CultureInfo.InvariantCulture);
                    float d = float.Parse(v[3], System.Globalization.CultureInfo.InvariantCulture);
                    textBlock.componentx = a;
                    textBlock.componenty = b;
                    textBlock.componentz = c;
                    textBlock.componentw = d;

                    if (textBlock.PropertyChanged != null)
                        textBlock.PropertyChanged.Invoke(textBlock, new System.ComponentModel.PropertyChangedEventArgs("ComponentX"));
                    if (textBlock.PropertyChanged != null)
                        textBlock.PropertyChanged.Invoke(textBlock, new System.ComponentModel.PropertyChangedEventArgs("ComponentY"));
                    if (textBlock.PropertyChanged != null)
                        textBlock.PropertyChanged.Invoke(textBlock, new System.ComponentModel.PropertyChangedEventArgs("ComponentZ"));
                    if (textBlock.PropertyChanged != null)
                        textBlock.PropertyChanged.Invoke(textBlock, new System.ComponentModel.PropertyChangedEventArgs("ComponentW"));
                }
            }
        }

        protected override void OnMouseDown(MouseButtonEventArgs e)
        {
            Focus();
            base.OnMouseDown(e);
            e.Handled = true;
        }


        #region INotifyPropertyChanged Members

        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;

        #endregion
    }


    public class TextField : Control, System.ComponentModel.INotifyPropertyChanged, IInputElement
    {
        public static readonly DependencyProperty TextProperty;

        static TextField()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(TextField), new FrameworkPropertyMetadata(typeof(TextField)));
            TextProperty = DependencyProperty.Register("Text", typeof(string), typeof(TextField),
                new FrameworkPropertyMetadata(string.Empty, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
        }

        public string Text
        {
            get
            {
                return GetValue(TextProperty) as string;
            }
            set
            {
                if (string.IsNullOrEmpty(value))
                    SetValue(TextProperty, string.Empty);
                else
                    SetValue(TextProperty, value);
            }
        }

        protected override void OnLostFocus(RoutedEventArgs e)
        {
            if (GetBindingExpression(TextProperty) != null)
                GetBindingExpression(TextProperty).UpdateSource();           
            base.OnLostFocus(e);
        }

        protected override void OnMouseDown(MouseButtonEventArgs e)
        {
            Focus();
            base.OnMouseDown(e);
            e.Handled = true;
        }


        #region INotifyPropertyChanged Members

        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;

        #endregion
    }

    public class EnumField : Control, System.ComponentModel.INotifyPropertyChanged, IInputElement
    {
        public static readonly DependencyProperty TextProperty;
        public static readonly DependencyProperty ItemsSourceProperty;

        static EnumField()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(EnumField), new FrameworkPropertyMetadata(typeof(EnumField)));
            TextProperty = DependencyProperty.Register("Text", typeof(string), typeof(EnumField),
                new FrameworkPropertyMetadata(string.Empty, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
            ItemsSourceProperty = DependencyProperty.Register("ItemsSource", typeof(object), typeof(EnumField),
                new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
            
        }

        public string Text
        {
            get
            {
                return GetValue(TextProperty) as string;
            }
            set
            {
                if (string.IsNullOrEmpty(value))
                    SetValue(TextProperty, string.Empty);
                else
                    SetValue(TextProperty, value);
            }
        }

        public object ItemsSource
        {
            get
            {
                return GetValue(ItemsSourceProperty);
            }
            set
            {
                SetValue(ItemsSourceProperty, value);                    
            }
        }

        protected override void OnLostFocus(RoutedEventArgs e)
        {
            if (GetBindingExpression(TextProperty) != null)
                GetBindingExpression(TextProperty).UpdateSource();
            base.OnLostFocus(e);
        }

        protected override void OnMouseDown(MouseButtonEventArgs e)
        {
            Focus();
            base.OnMouseDown(e);
            e.Handled = true;
        }


        #region INotifyPropertyChanged Members

        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;

        #endregion
    }

    public class BooleanField : Control, System.ComponentModel.INotifyPropertyChanged, IInputElement
    {
        public static readonly DependencyProperty TextProperty;

        static BooleanField()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(BooleanField), new FrameworkPropertyMetadata(typeof(BooleanField)));
            TextProperty = DependencyProperty.Register("Text", typeof(string), typeof(BooleanField),
                new FrameworkPropertyMetadata(string.Empty, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
        }

        public string Text
        {
            get
            {
                return GetValue(TextProperty) as string;
            }
            set
            {
                if (string.IsNullOrEmpty(value))
                    SetValue(TextProperty, string.Empty);
                else
                    SetValue(TextProperty, value);
            }
        }

        protected override void OnLostFocus(RoutedEventArgs e)
        {
            if(GetBindingExpression(TextProperty) != null)
                GetBindingExpression(TextProperty).UpdateSource();
            base.OnLostFocus(e);
        }

        protected override void OnMouseDown(MouseButtonEventArgs e)
        {
            Focus();
            base.OnMouseDown(e);
            e.Handled = true;
        }


        #region INotifyPropertyChanged Members

        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;

        #endregion
    }

    public class BooleanFieldDisplayConverter : IValueConverter
    {
        #region IValueConverter Members

        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            if (value is string)
            {
                int a = 0;
                int.TryParse(value as string, System.Globalization.NumberStyles.Integer, System.Globalization.CultureInfo.InvariantCulture, out a);
                if (a == 0) return "False";
                return "True";
            }

            return "False";
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }

        #endregion
    }

    public class BooleanFieldValueConverter : IValueConverter
    {
        #region IValueConverter Members

        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            if (value is string)
            {
                int a = 0;
                int.TryParse(value as string, System.Globalization.NumberStyles.Integer, System.Globalization.CultureInfo.InvariantCulture, out a);
                if (a == 0) return false;
                return true;
            }

            return false;
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            bool v = (bool)value;
            if (v == true) return "1";
            return "0";
        }

        #endregion
    }
}
