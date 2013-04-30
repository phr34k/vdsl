using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Media;
using System.Windows.Controls;

namespace Aurora.SandboxDesigner.Controls
{
    public class ResizeableCanvas : Panel
    {
        #region Public Members

        // Fields
        public static readonly DependencyProperty BottomProperty;
        public static readonly DependencyProperty LeftProperty;
        public static readonly DependencyProperty RightProperty;
        public static readonly DependencyProperty TopProperty;

        #endregion

        #region Private Methods

        // Methods
        protected override Size ArrangeOverride(Size arrangeSize)
        {
            foreach (UIElement element in base.InternalChildren)
            {
                if (element == null)
                {
                    continue;
                }
                double x = 0.0;
                double y = 0.0;
                double left = GetLeft(element);
                if (!Double.IsNaN(left))
                {
                    x = left;
                }
                else
                {
                    double right = GetRight(element);
                    if (!Double.IsNaN(right))
                    {
                        x = (arrangeSize.Width - element.DesiredSize.Width) - right;
                    }
                }
                double top = GetTop(element);
                if (!Double.IsNaN(top))
                {
                    y = top;
                }
                else
                {
                    double bottom = GetBottom(element);
                    if (!Double.IsNaN(bottom))
                    {
                        y = (arrangeSize.Height - element.DesiredSize.Height) - bottom;
                    }
                }
                element.Arrange(new Rect(new Point(x, y), element.DesiredSize));
            }
            return arrangeSize;
        }

        protected override Geometry GetLayoutClip(Size layoutSlotSize)
        {
            if (base.ClipToBounds)
            {
                return new RectangleGeometry(new Rect(base.RenderSize));
            }
            return null;
        }

        internal static bool IsDoubleFiniteOrNaN(object o)
        {
            double d = (double)o;
            return !double.IsInfinity(d);
        }

        protected override Size MeasureOverride(Size constraint)
        {
            Size availableSize = new Size(double.PositiveInfinity, double.PositiveInfinity);
            double maxHeight = 0;
            double maxWidth = 0;

            foreach (UIElement element in base.InternalChildren)
            {
                if (element != null)
                {
                    element.Measure(availableSize);
                    double left = GetLeft(element);
                    double top = GetTop(element);

                    left += element.DesiredSize.Width;
                    top += element.DesiredSize.Height;
                    maxWidth = maxWidth < left ? left : maxWidth;
                    maxHeight = maxHeight < top ? top : maxHeight;

                }
            }





            return new Size
            {
                Height = (Math.Ceiling(maxHeight / 2048.0f) * 2048.0f) + 128.0f,
                Width = (Math.Ceiling(maxWidth / 2048.0f) * 2048.0f) + 128.0f
            };
        }

        private static void OnPositioningChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            UIElement reference = d as UIElement;
            if (reference != null)
            {
                ResizeableCanvas parent = VisualTreeHelper.GetParent(reference) as ResizeableCanvas;
                if (parent != null)
                {
                    parent.InvalidateMeasure();
                    parent.InvalidateArrange();
                }
            }
        }

        #endregion

        #region Public Methods

        public static double GetBottom(UIElement element)
        {
            if (element == null)
            {
                throw new ArgumentNullException("element");
            }
            return (double)element.GetValue(BottomProperty);
        }

        public static double GetLeft(UIElement element)
        {
            if (element == null)
            {
                throw new ArgumentNullException("element");
            }
            return (double)element.GetValue(LeftProperty);
        }

        public static double GetRight(UIElement element)
        {
            if (element == null)
            {
                throw new ArgumentNullException("element");
            }
            return (double)element.GetValue(RightProperty);
        }

        public static double GetTop(UIElement element)
        {
            if (element == null)
            {
                throw new ArgumentNullException("element");
            }
            return (double)element.GetValue(TopProperty);
        }

        public static void SetBottom(UIElement element, double length)
        {
            if (element == null)
            {
                throw new ArgumentNullException("element");
            }
            element.SetValue(BottomProperty, length);
        }

        public static void SetLeft(UIElement element, double length)
        {
            if (element == null)
            {
                throw new ArgumentNullException("element");
            }
            element.SetValue(LeftProperty, length);
        }

        public static void SetRight(UIElement element, double length)
        {
            if (element == null)
            {
                throw new ArgumentNullException("element");
            }
            element.SetValue(RightProperty, length);
        }

        public static void SetTop(UIElement element, double length)
        {
            if (element == null)
            {
                throw new ArgumentNullException("element");
            }
            element.SetValue(TopProperty, length);
        }

        #endregion

        #region Ctor / Dtor

        static ResizeableCanvas()
        {
            try
            {
                /*
                BottomProperty = Canvas.BottomProperty.AddOwner(typeof(ResizeableCanvas), new FrameworkPropertyMetadata(Double.NaN, new PropertyChangedCallback(ResizeableCanvas.OnPositioningChanged)));
                LeftProperty = Canvas.LeftProperty.AddOwner(typeof(ResizeableCanvas), new FrameworkPropertyMetadata(Double.NaN, new PropertyChangedCallback(ResizeableCanvas.OnPositioningChanged)));
                RightProperty = Canvas.RightProperty.AddOwner(typeof(ResizeableCanvas), new FrameworkPropertyMetadata(Double.NaN, new PropertyChangedCallback(ResizeableCanvas.OnPositioningChanged)));
                TopProperty = Canvas.TopProperty.AddOwner(typeof(ResizeableCanvas), new FrameworkPropertyMetadata(Double.NaN, new PropertyChangedCallback(ResizeableCanvas.OnPositioningChanged)));
                 * */
                

                LeftProperty = DependencyProperty.RegisterAttached("Left", typeof(double), typeof(ResizeableCanvas), new FrameworkPropertyMetadata(Double.NaN, new PropertyChangedCallback(ResizeableCanvas.OnPositioningChanged)), new ValidateValueCallback(ResizeableCanvas.IsDoubleFiniteOrNaN));
                TopProperty = DependencyProperty.RegisterAttached("Top", typeof(double), typeof(ResizeableCanvas), new FrameworkPropertyMetadata(Double.NaN, new PropertyChangedCallback(ResizeableCanvas.OnPositioningChanged)), new ValidateValueCallback(ResizeableCanvas.IsDoubleFiniteOrNaN));
                RightProperty = DependencyProperty.RegisterAttached("Right", typeof(double), typeof(ResizeableCanvas), new FrameworkPropertyMetadata(Double.NaN, new PropertyChangedCallback(ResizeableCanvas.OnPositioningChanged)), new ValidateValueCallback(ResizeableCanvas.IsDoubleFiniteOrNaN));
                BottomProperty = DependencyProperty.RegisterAttached("Bottom", typeof(double), typeof(ResizeableCanvas), new FrameworkPropertyMetadata(Double.NaN, new PropertyChangedCallback(ResizeableCanvas.OnPositioningChanged)), new ValidateValueCallback(ResizeableCanvas.IsDoubleFiniteOrNaN));
            }
            catch (Exception ex)
            {
            }
        }

        #endregion
    }
}
