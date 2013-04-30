using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows;
using System.Windows.Input;
using System.Windows.Data;
using Aurora.SandboxDesigner.Internals;

namespace Aurora.SandboxDesigner.Controls
{
    public enum CurveLoopType
    {
        Constant,
        Cycle,
        CycleOffset,
        Oscillate,
        Linear
    }

    public enum CurveTangent
    {
        Flat,
        Linear,
        Smooth
    }

    public enum CurveContinuity
    {
        Smooth,
        Step
    }

    [Serializable, TypeConverter(typeof(ExpandableObjectConverter))]
    public class CurveKey : IEquatable<CurveKey>, IComparable<CurveKey>
    {
        #region Internals

        internal CurveContinuity continuity;
        internal float internalValue;
        internal float position;
        internal float tangentIn;
        internal float tangentOut;

        #endregion

        #region Public Members

        public CurveKey Clone()
        {
            return new CurveKey(this.position, this.internalValue, this.tangentIn, this.tangentOut, this.continuity);
        }

        public int CompareTo(CurveKey other)
        {
            if (this.position == other.position)
            {
                return 0;
            }
            if (this.position >= other.position)
            {
                return 1;
            }
            return -1;
        }

        public bool Equals(CurveKey other)
        {
            return (((((other != null) && (other.position == this.position)) && ((other.internalValue == this.internalValue) && (other.tangentIn == this.tangentIn))) && (other.tangentOut == this.tangentOut)) && (other.continuity == this.continuity));
        }

        public override bool Equals(object obj)
        {
            return this.Equals(obj as CurveKey);
        }

        public override int GetHashCode()
        {
            return ((((this.position.GetHashCode() + this.internalValue.GetHashCode()) + this.tangentIn.GetHashCode()) + this.tangentOut.GetHashCode()) + this.continuity.GetHashCode());
        }

        public static bool operator ==(CurveKey a, CurveKey b)
        {
            bool flag3 = null == (object)a;
            bool flag2 = null == (object)b;
            if (flag3 || flag2)
                return (flag3 == flag2);
            return a.Equals(b);
        }

        public static bool operator !=(CurveKey a, CurveKey b)
        {
            bool flag3 = null == (object)a;
            bool flag2 = null == (object)b;
            if (flag3 || flag2)
            {
                return (flag3 != flag2);
            }
            return ((((a.position != b.position) || (a.internalValue != b.internalValue)) || ((a.tangentIn != b.tangentIn) || (a.tangentOut != b.tangentOut))) || (a.continuity != b.continuity));
        }


        public CurveContinuity Continuity
        {
            get
            {
                return this.continuity;
            }
            set
            {
                this.continuity = value;
            }
        }

        public float Position
        {
            get
            {
                return this.position;
            }
        }

        public float TangentIn
        {
            get
            {
                return this.tangentIn;
            }
            set
            {
                this.tangentIn = value;
            }
        }

        public float TangentOut
        {
            get
            {
                return this.tangentOut;
            }
            set
            {
                this.tangentOut = value;
            }
        }

        public float Value
        {
            get
            {
                return this.internalValue;
            }
            set
            {
                this.internalValue = value;
            }
        }

        #endregion

        #region Ctor / Dtor

        public CurveKey(float position, float value)
        {
            this.position = position;
            this.internalValue = value;
        }

        public CurveKey(float position, float value, float tangentIn, float tangentOut)
        {
            this.position = position;
            this.internalValue = value;
            this.tangentIn = tangentIn;
            this.tangentOut = tangentOut;
        }

        public CurveKey(float position, float value, float tangentIn, float tangentOut, CurveContinuity continuity)
        {
            this.position = position;
            this.internalValue = value;
            this.tangentIn = tangentIn;
            this.tangentOut = tangentOut;
            this.continuity = continuity;
        }

        #endregion
    }

    [Serializable, TypeConverter(typeof(ExpandableObjectConverter))]
    public class CurveKeyCollection : ICollection<CurveKey>, IEnumerable<CurveKey>, System.Collections.IEnumerable
    {
        #region Internal Members

        internal float InvTimeRange;
        internal bool IsCacheAvailable = true;
        internal float TimeRange;

        internal void ComputeCacheValues()
        {
            this.TimeRange = this.InvTimeRange = 0f;
            if (this.Keys.Count > 1)
            {
                this.TimeRange = this.Keys[this.Keys.Count - 1].Position - this.Keys[0].Position;
                if (this.TimeRange > float.Epsilon)
                {
                    this.InvTimeRange = 1f / this.TimeRange;
                }
            }
            this.IsCacheAvailable = true;
        }

        #endregion

        #region Private Members

        private List<CurveKey> Keys = new List<CurveKey>();

        #endregion

        #region Public Members

        public void Add(CurveKey item)
        {
            if (item == null)
            {
                throw new ArgumentNullException();
            }
            int index = this.Keys.BinarySearch(item);
            if (index >= 0)
            {
                while ((index < this.Keys.Count) && (item.Position == this.Keys[index].Position))
                {
                    index++;
                }
            }
            else
            {
                index = ~index;
            }
            this.Keys.Insert(index, item);
            this.IsCacheAvailable = false;
        }

        public void Clear()
        {
            this.Keys.Clear();
            this.TimeRange = this.InvTimeRange = 0f;
            this.IsCacheAvailable = false;
        }

        public CurveKeyCollection Clone()
        {
            return new CurveKeyCollection { Keys = new List<CurveKey>(this.Keys), InvTimeRange = this.InvTimeRange, TimeRange = this.TimeRange, IsCacheAvailable = true };
        }

        public bool Contains(CurveKey item)
        {
            return this.Keys.Contains(item);
        }

        public void CopyTo(CurveKey[] array, int arrayIndex)
        {
            this.Keys.CopyTo(array, arrayIndex);
            this.IsCacheAvailable = false;
        }

        public IEnumerator<CurveKey> GetEnumerator()
        {
            return this.Keys.GetEnumerator();
        }

        public int IndexOf(CurveKey item)
        {
            return this.Keys.IndexOf(item);
        }

        public bool Remove(CurveKey item)
        {
            this.IsCacheAvailable = false;
            return this.Keys.Remove(item);
        }

        public void RemoveAt(int index)
        {
            this.Keys.RemoveAt(index);
            this.IsCacheAvailable = false;
        }

        public int Count
        {
            get
            {
                return this.Keys.Count;
            }
        }

        public bool IsReadOnly
        {
            get
            {
                return false;
            }
        }

        public CurveKey this[int index]
        {
            get
            {
                return this.Keys[index];
            }
            set
            {
                if (value == null)
                {
                    throw new ArgumentNullException();
                }
                if (this.Keys[index].Position == value.Position)
                {
                    this.Keys[index] = value;
                }
                else
                {
                    this.Keys.RemoveAt(index);
                    this.Add(value);
                }
            }
        }

        #endregion

        #region IEnumerable

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return this.Keys.GetEnumerator();
        }

        #endregion
    }

    [Serializable, TypeConverter(typeof(ExpandableObjectConverter))]
    public class Curve
    {
        #region Private Members

        private CurveKeyCollection keys = new CurveKeyCollection();
        private CurveLoopType postLoop;
        private CurveLoopType preLoop;
        private float CalcCycle(float t)
        {
            float num = (t - this.keys[0].position) * this.keys.InvTimeRange;
            if (num < 0f)
            {
                num--;
            }
            int num2 = (int)num;
            return (float)num2;
        }
        private float FindSegment(float t, ref CurveKey k0, ref CurveKey k1)
        {
            float num2 = t;
            k0 = this.keys[0];
            for (int i = 1; i < this.keys.Count; i++)
            {
                k1 = this.keys[i];
                if (k1.position >= t)
                {
                    double position = k0.position;
                    double num6 = k1.position;
                    double num5 = t;
                    double num3 = num6 - position;
                    num2 = 0f;
                    if (num3 > 1E-10)
                    {
                        num2 = (float)((num5 - position) / num3);
                    }
                    return num2;
                }
                k0 = k1;
            }
            return num2;
        }
        private static float Hermite(CurveKey k0, CurveKey k1, float t)
        {
            if (k0.Continuity == CurveContinuity.Step)
            {
                if (t >= 1f)
                {
                    return k1.internalValue;
                }
                return k0.internalValue;
            }
            float num = t * t;
            float num2 = num * t;
            float internalValue = k0.internalValue;
            float num5 = k1.internalValue;
            float tangentOut = k0.tangentOut;
            float tangentIn = k1.tangentIn;
            return ((((internalValue * (((2f * num2) - (3f * num)) + 1f)) + (num5 * ((-2f * num2) + (3f * num)))) + (tangentOut * ((num2 - (2f * num)) + t))) + (tangentIn * (num2 - num)));
        }

        #endregion

        #region Public Members

        public Curve Clone()
        {
            return new Curve { preLoop = this.preLoop, postLoop = this.postLoop, keys = this.keys.Clone() };
        }

        public void ComputeTangent(int keyIndex, CurveTangent tangentType)
        {
            this.ComputeTangent(keyIndex, tangentType, tangentType);
        }

        public void ComputeTangent(int keyIndex, CurveTangent tangentInType, CurveTangent tangentOutType)
        {
            float num2;
            float num4;
            float num7;
            float num8;
            if ((this.keys.Count <= keyIndex) || (keyIndex < 0))
            {
                throw new ArgumentOutOfRangeException("keyIndex");
            }
            CurveKey key = this.Keys[keyIndex];
            float position = num8 = num4 = key.Position;
            float num = num7 = num2 = key.Value;
            if (keyIndex > 0)
            {
                position = this.Keys[keyIndex - 1].Position;
                num = this.Keys[keyIndex - 1].Value;
            }
            if ((keyIndex + 1) < this.keys.Count)
            {
                num4 = this.Keys[keyIndex + 1].Position;
                num2 = this.Keys[keyIndex + 1].Value;
            }
            if (tangentInType == CurveTangent.Smooth)
            {
                float num10 = num4 - position;
                float num6 = num2 - num;
                if (Math.Abs(num6) < 1.192093E-07f)
                {
                    key.TangentIn = 0f;
                }
                else
                {
                    key.TangentIn = (num6 * Math.Abs((float)(position - num8))) / num10;
                }
            }
            else if (tangentInType == CurveTangent.Linear)
            {
                key.TangentIn = num7 - num;
            }
            else
            {
                key.TangentIn = 0f;
            }
            if (tangentOutType == CurveTangent.Smooth)
            {
                float num9 = num4 - position;
                float num5 = num2 - num;
                if (Math.Abs(num5) < 1.192093E-07f)
                {
                    key.TangentOut = 0f;
                }
                else
                {
                    key.TangentOut = (num5 * Math.Abs((float)(num4 - num8))) / num9;
                }
            }
            else if (tangentOutType == CurveTangent.Linear)
            {
                key.TangentOut = num2 - num7;
            }
            else
            {
                key.TangentOut = 0f;
            }
        }

        public void ComputeTangents(CurveTangent tangentType)
        {
            this.ComputeTangents(tangentType, tangentType);
        }

        public void ComputeTangents(CurveTangent tangentInType, CurveTangent tangentOutType)
        {
            for (int i = 0; i < this.Keys.Count; i++)
            {
                this.ComputeTangent(i, tangentInType, tangentOutType);
            }
        }

        public float Evaluate(float position)
        {
            if (this.keys.Count == 0)
            {
                return 0f;
            }
            if (this.keys.Count == 1)
            {
                return this.keys[0].internalValue;
            }
            CurveKey key = this.keys[0];
            CurveKey key2 = this.keys[this.keys.Count - 1];
            float t = position;
            float num6 = 0f;
            if (t < key.position)
            {
                if (this.preLoop == CurveLoopType.Constant)
                {
                    return key.internalValue;
                }
                if (this.preLoop == CurveLoopType.Linear)
                {
                    return (key.internalValue - (key.tangentIn * (key.position - t)));
                }
                if (!this.keys.IsCacheAvailable)
                {
                    this.keys.ComputeCacheValues();
                }
                float num5 = this.CalcCycle(t);
                float num3 = t - (key.position + (num5 * this.keys.TimeRange));
                if (this.preLoop == CurveLoopType.Cycle)
                {
                    t = key.position + num3;
                }
                else if (this.preLoop == CurveLoopType.CycleOffset)
                {
                    t = key.position + num3;
                    num6 = (key2.internalValue - key.internalValue) * num5;
                }
                else
                {
                    t = ((((int)num5) & 1) != 0) ? (key2.position - num3) : (key.position + num3);
                }
            }
            else if (key2.position < t)
            {
                if (this.postLoop == CurveLoopType.Constant)
                {
                    return key2.internalValue;
                }
                if (this.postLoop == CurveLoopType.Linear)
                {
                    return (key2.internalValue - (key2.tangentOut * (key2.position - t)));
                }
                if (!this.keys.IsCacheAvailable)
                {
                    this.keys.ComputeCacheValues();
                }
                float num4 = this.CalcCycle(t);
                float num2 = t - (key.position + (num4 * this.keys.TimeRange));
                if (this.postLoop == CurveLoopType.Cycle)
                {
                    t = key.position + num2;
                }
                else if (this.postLoop == CurveLoopType.CycleOffset)
                {
                    t = key.position + num2;
                    num6 = (key2.internalValue - key.internalValue) * num4;
                }
                else
                {
                    t = ((((int)num4) & 1) != 0) ? (key2.position - num2) : (key.position + num2);
                }
            }
            CurveKey key4 = null;
            CurveKey key3 = null;
            t = this.FindSegment(t, ref key4, ref key3);
            return (num6 + Hermite(key4, key3, t));
        }

        public bool IsConstant
        {
            get
            {
                return (this.keys.Count <= 1);
            }
        }
        public CurveKeyCollection Keys
        {
            get
            {
                return this.keys;
            }
        }
        public CurveLoopType PostLoop
        {
            get
            {
                return this.postLoop;
            }
            set
            {
                this.postLoop = value;
            }
        }
        public CurveLoopType PreLoop
        {
            get
            {
                return this.preLoop;
            }
            set
            {
                this.preLoop = value;
            }
        }

        #endregion
    }

    public class AnimationChannel : INotifyPropertyChanged
    {
        #region Private Members

        private bool visible = true;
        private CurveTangent easying = CurveTangent.Smooth;
        private float value;
        private System.Windows.Media.Color color;

        #endregion

        #region Public Members

        public Curve curve1 = null;
        public Curve curve2 = null;

        public string Department
        { get; set; }

        public string Name
        { get; set; }

        public float Minimum
        {
            get;
            set;
        }

        public float Maximum
        {
            get;
            set;
        }

        public bool Visible
        {
            get
            {
                return visible;
            }
            set
            {
                if (visible != value)
                {
                    visible = value;
                    if (PropertyChanged != null) PropertyChanged.Invoke(this, new PropertyChangedEventArgs("Visible"));
                }
            }
        }

        public CurveTangent Easying
        {
            get
            {
                return easying;
            }
            set
            {
                if (easying != value)
                {
                    easying = value;
                    if (PropertyChanged != null) PropertyChanged.Invoke(this, new PropertyChangedEventArgs("Easying"));
                }
            }
        }

        public float Value
        {
            get
            {
                return value;
            }
            set
            {
                if (this.value != value)
                {
                    this.value = value;
                    if (PropertyChanged != null) PropertyChanged.Invoke(this, new PropertyChangedEventArgs("Value"));
                }
            }
        }

        public System.Windows.Media.Color Color
        {
            get
            {
                return color;
            }
            set
            {
                if (this.color != value)
                {
                    this.color = value;
                    if (PropertyChanged != null) PropertyChanged.Invoke(this, new PropertyChangedEventArgs("Color"));
                }
            }
        }


        #endregion

        #region INotifyPropertyChanged Members

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion

        #region INotifyPropertyChanged Members

        event PropertyChangedEventHandler INotifyPropertyChanged.PropertyChanged
        {
            add { PropertyChanged += value; }
            remove { PropertyChanged -= value; }
        }

        #endregion

        #region Ctor/Dtor

        public AnimationChannel()
        {
            curve1 = new Curve();
            curve2 = new Curve();
        }

        #endregion
    }

    public class AnimationEditorGrid : Grid
    {
        protected override Geometry GetLayoutClip(System.Windows.Size layoutSlotSize)
        {
            RectangleGeometry geo = base.GetLayoutClip(layoutSlotSize) as RectangleGeometry;
            if (geo != null)
            {
                System.Windows.Rect newBounds = geo.Bounds;
                newBounds.Y -= 24;
                newBounds.Height += 24;
                return new RectangleGeometry(newBounds, geo.RadiusX, geo.RadiusY, geo.Transform);
            }
            else
            {
                return null;
            }
        }
    }

    public class AnimationTimeIndicator : Control
    {
        #region Private Members

        private bool istracing = false;

        #endregion

        #region Public Members

        public static readonly DependencyProperty MinimumProperty = DependencyProperty.Register("Minimum",
             typeof(float), typeof(AnimationTimeIndicator), new FrameworkPropertyMetadata(0.0f, FrameworkPropertyMetadataOptions.AffectsRender));
        public static readonly DependencyProperty MaximumProperty = DependencyProperty.Register("Maximum",
             typeof(float), typeof(AnimationTimeIndicator), new FrameworkPropertyMetadata(0.0f, FrameworkPropertyMetadataOptions.AffectsRender));
        public static readonly DependencyProperty OffsetProperty = DependencyProperty.Register("Offset",
             typeof(float), typeof(AnimationTimeIndicator), new FrameworkPropertyMetadata(0.0f, FrameworkPropertyMetadataOptions.AffectsRender));
        public static readonly DependencyProperty ValueProperty = DependencyProperty.Register("Value",
             typeof(float), typeof(AnimationTimeIndicator), new FrameworkPropertyMetadata(0.0f, FrameworkPropertyMetadataOptions.AffectsRender | FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
        public static readonly DependencyProperty ScaleProperty = DependencyProperty.Register("Scale",
            typeof(float), typeof(AnimationTimeIndicator), new FrameworkPropertyMetadata(100.0f, FrameworkPropertyMetadataOptions.AffectsRender | FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));


        public float Minimum
        {
            get
            {
                return (float)this.GetValue(MinimumProperty);
            }
            set
            {
                this.SetValue(MinimumProperty, value);
            }
        }

        public float Maximum
        {
            get
            {
                return (float)this.GetValue(MaximumProperty);
            }
            set
            {
                this.SetValue(MaximumProperty, value);
            }
        }

        public float Offset
        {
            get
            {
                return (float)this.GetValue(OffsetProperty);
            }
            set
            {
                this.SetValue(OffsetProperty, value);
            }
        }

        public float Value
        {
            get
            {
                return (float)this.GetValue(ValueProperty);
            }
            set
            {
                this.SetValue(ValueProperty, value);
            }
        }

        public float Scale
        {
            get
            {
                return (float)this.GetValue(ScaleProperty);
            }
            set
            {
                this.SetValue(ScaleProperty, value);
            }
        }


        #endregion

        #region Protected Members

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            istracing = Mouse.Capture(this);
        }

        protected override void OnMouseLeftButtonUp(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonUp(e);
            istracing = false;
            Mouse.Capture(this, CaptureMode.None);
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            if (istracing)
            {
                Value = (int)((Offset + e.GetPosition(this).X) * 100 / Scale);
                if (Value < Minimum) Value = Minimum;
                if (Value > Maximum) Value = Maximum;

                InvalidateVisual();
            }
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            base.OnRender(drawingContext);

            Pen blackPen = new Pen();
            blackPen.Thickness = 0.2;
            blackPen.LineJoin = PenLineJoin.Bevel;
            blackPen.StartLineCap = PenLineCap.Triangle;
            blackPen.EndLineCap = PenLineCap.Round;
            blackPen.Brush = Brushes.Red;

            Pen blackPen2 = new Pen();
            blackPen2.Thickness = 1;
            blackPen2.LineJoin = PenLineJoin.Bevel;
            blackPen2.StartLineCap = PenLineCap.Triangle;
            blackPen2.EndLineCap = PenLineCap.Round;
            blackPen2.Brush = Brushes.Transparent;

            float p = -Offset + (Value) / 100.0f * Scale;
            drawingContext.DrawLine(blackPen2, new Point(p - 1, 0), new Point(p - 1, ActualHeight));
            drawingContext.DrawLine(blackPen, new Point(p, 0), new Point(p, ActualHeight));
            drawingContext.DrawLine(blackPen2, new Point(p + 1, 0), new Point(p + 1, ActualHeight));
        }

        #endregion

        #region Ctor / Dtor

        static AnimationTimeIndicator()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(AnimationTimeIndicator), new FrameworkPropertyMetadata(typeof(AnimationTimeIndicator)));
        }

        public AnimationTimeIndicator()
        {
            Cursor = Cursors.SizeWE;
        }

        #endregion
    }

    public class AnimationEditor : ItemsControl
    {

        #region Private Members





        private System.Collections.ObjectModel.ObservableCollection<object> visibleItems =
                            new System.Collections.ObjectModel.ObservableCollection<object>();

        private static void OnAnimationCurrentTimeChanged(DependencyObject o, DependencyPropertyChangedEventArgs e)
        {
            double time = (o as AnimationEditor).AnimationCurrentTime;
            foreach (AnimationChannel tweenable in (o as AnimationEditor).Items)
                tweenable.Value = tweenable.curve1.Evaluate((float)time);
            if ((o as AnimationEditor).OnTimeChanged != null)
                (o as AnimationEditor).OnTimeChanged.Invoke(o, EventArgs.Empty);
        }

        private static void OnAnimationDurationChanged(DependencyObject o, DependencyPropertyChangedEventArgs e)
        {
            AnimationEditor sender = o as AnimationEditor;
            sender.AnimationViewportOffsetWidth = sender.AnimationDuration / 100.0f * sender.AnimationViewportScale;
        }

        private static void OnAnimationScaleChanged(DependencyObject o, DependencyPropertyChangedEventArgs e)
        {
            AnimationEditor sender = o as AnimationEditor;
            sender.AnimationViewportOffsetWidth = sender.AnimationDuration / 100.0f * sender.AnimationViewportScale;
        }


        private void AnimationEditor_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Visible")
            {
                if (IsVisible(sender))
                {
                    visibleItems.Add(sender);
                }
                else
                {
                    visibleItems.Remove(sender);
                }
            }
        }

        private void Part_Easing_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            /*
            DataGrid Part_AnimationChannels = Template.FindName("Part_AnimationChannels", this) as DataGrid;
            AnimationChannel item = Part_AnimationChannels.SelectedItem as AnimationChannel;
            if (item != null)
            {
                item.curve1.ComputeTangents(item.Easying);
                item.curve2.ComputeTangents(item.Easying);
                NotifyCurveInvalidated(item);
            }
            */
        }

        private void Part_Continuity_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            /*
            if (SelectedKeyframeIndex > -1 && SelectedItemIndex > -1)
            {
                DataGrid Part_AnimationChannels = Template.FindName("Part_AnimationChannels", this) as DataGrid;
                AnimationChannel item = Items[SelectedItemIndex] as AnimationChannel;
                item.curve1.Keys[SelectedKeyframeIndex].Continuity = (CurveContinuity)(sender as ComboBox).SelectedIndex;
                item.curve2.Keys[SelectedKeyframeIndex].Continuity = (CurveContinuity)(sender as ComboBox).SelectedIndex;
                NotifyCurveInvalidated(item);
            }
            */
        }

        private void Part_AnimationChannels_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.AddedItems.Count > 0)
            {
                AnimationChannel tweenable = e.AddedItems[e.AddedItems.Count - 1] as AnimationChannel;
            }
        }

        private void RoutedCommand_SelectColor_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            /*
            SelectColorDialog dialog = new SelectColorDialog();
            if (dialog.ShowDialog() == true)
            {
                AnimationChannel channel = (e.OriginalSource as FrameworkElement).DataContext as AnimationChannel;
                System.Reflection.PropertyInfo info = (System.Reflection.PropertyInfo)dialog.Part_ColorListBox.SelectedItem;
                channel.Color = (System.Windows.Media.Color)info.GetValue(null, new object[] { });
                NotifyCurveInvalidated(channel);
            }
            */
        }

        #endregion

        #region Public Members

        public event TimeChangedEventHandler OnTimeChanged = null;
        public static readonly DependencyProperty AnimationViewportScaleProperty = DependencyProperty.Register("AnimationViewportScale",
                            typeof(double), typeof(AnimationEditor), new FrameworkPropertyMetadata((double)100.0f, FrameworkPropertyMetadataOptions.AffectsRender | FrameworkPropertyMetadataOptions.BindsTwoWayByDefault,
                            new PropertyChangedCallback(OnAnimationScaleChanged)));
        public static readonly DependencyProperty AnimationCurrentTimeProperty = DependencyProperty.Register("AnimationCurrentTime",
                            typeof(double), typeof(AnimationEditor), new FrameworkPropertyMetadata((double)0.0f, FrameworkPropertyMetadataOptions.AffectsRender | FrameworkPropertyMetadataOptions.BindsTwoWayByDefault,
                            new PropertyChangedCallback(OnAnimationCurrentTimeChanged)));
        public static readonly DependencyProperty AnimationDurationProperty = DependencyProperty.Register("AnimationDuration",
                            typeof(float), typeof(AnimationEditor), new FrameworkPropertyMetadata((float)0.0f, FrameworkPropertyMetadataOptions.AffectsRender | FrameworkPropertyMetadataOptions.BindsTwoWayByDefault,
                            new PropertyChangedCallback(OnAnimationDurationChanged)));
        public static readonly DependencyProperty AnimationViewportOffsetProperty = DependencyProperty.Register("AnimationViewportOffset",
                            typeof(double), typeof(AnimationEditor));
        public static readonly DependencyProperty AnimationViewportOffsetWidthProperty = DependencyProperty.Register("AnimationViewportOffsetWidth",
                            typeof(double), typeof(AnimationEditor), new FrameworkPropertyMetadata((double)2400.0f, FrameworkPropertyMetadataOptions.None, null));
        public static readonly DependencyProperty SelectedItemIndexProperty = DependencyProperty.Register("SelectedItemIndex",
                            typeof(int), typeof(AnimationEditor),
                            new FrameworkPropertyMetadata(-1, FrameworkPropertyMetadataOptions.AffectsRender, null));
        public static readonly DependencyProperty SelectedKeyframeIndexProperty = DependencyProperty.Register("SelectedKeyframeIndex",
                            typeof(int), typeof(AnimationEditor),
                            new FrameworkPropertyMetadata(-1, FrameworkPropertyMetadataOptions.AffectsRender, null));

        public float AnimationDuration
        {
            get
            {
                return (float)this.GetValue(AnimationDurationProperty);
            }
            set
            {
                this.SetValue(AnimationDurationProperty, value);
            }
        }

        public double AnimationCurrentTime
        {
            get
            {
                return (double)this.GetValue(AnimationCurrentTimeProperty);
            }
            set
            {
                this.SetValue(AnimationCurrentTimeProperty, value);
            }
        }

        public double AnimationViewportOffset
        {
            get
            {
                return (double)this.GetValue(AnimationViewportOffsetProperty);
            }
            set
            {
                this.SetValue(AnimationViewportOffsetProperty, value);
            }
        }

        public double AnimationViewportOffsetWidth
        {
            get
            {
                return (double)this.GetValue(AnimationViewportOffsetWidthProperty);
            }
            set
            {
                this.SetValue(AnimationViewportOffsetWidthProperty, value);
            }
        }

        public double AnimationViewportScale
        {
            get
            {
                return (double)this.GetValue(AnimationViewportScaleProperty);
            }
            set
            {
                this.SetValue(AnimationViewportScaleProperty, value);
            }
        }

        public int SelectedItemIndex
        {
            get
            {
                return (int)this.GetValue(SelectedItemIndexProperty);
            }
            set
            {
                this.SetValue(SelectedItemIndexProperty, value);

                /*
                DataGrid Part_AnimationChannels = Template.FindName("Part_AnimationChannels", this) as DataGrid;
                Part_AnimationChannels.ApplyTemplate();
                ScrollViewer viewer = Part_AnimationChannels.Template.FindName("Part_ScrollViewer", Part_AnimationChannels) as ScrollViewer;
                viewer.ApplyTemplate();
                ComboBox Part_Continuity = viewer.Template.FindName("Part_Continuity", viewer) as ComboBox;

                Part_Continuity.SelectionChanged -= new SelectionChangedEventHandler(Part_Continuity_SelectionChanged);
                if (SelectedItemIndex == -1)
                {
                    Part_Continuity.SelectedIndex = -1;
                }
                else if (SelectedKeyframeIndex == -1)
                {
                    Part_Continuity.SelectedIndex = -1;
                }
                else if (SelectedKeyframeIndex >= (Items[SelectedItemIndex] as AnimationChannel).curve1.Keys.Count)
                {
                    Part_Continuity.SelectedIndex = -1;
                }
                else
                {
                    int si = SelectedItemIndex;
                    int ki = SelectedKeyframeIndex;
                    AnimationChannel tweenable = Items[si] as AnimationChannel;
                    Part_Continuity.SelectedIndex = (int)tweenable.curve1.Keys[ki].Continuity;
                }

                Part_Continuity.SelectionChanged += new SelectionChangedEventHandler(Part_Continuity_SelectionChanged);
                 * */










            }
        }

        public int SelectedKeyframeIndex
        {
            get
            {
                return (int)this.GetValue(SelectedKeyframeIndexProperty);
            }
            set
            {
                this.SetValue(SelectedKeyframeIndexProperty, value);

                /*
                DataGrid Part_AnimationChannels = Template.FindName("Part_AnimationChannels", this) as DataGrid;
                Part_AnimationChannels.ApplyTemplate();
                ScrollViewer viewer = Part_AnimationChannels.Template.FindName("Part_ScrollViewer", Part_AnimationChannels) as ScrollViewer;
                viewer.ApplyTemplate();
                ComboBox Part_Continuity = viewer.Template.FindName("Part_Continuity", viewer) as ComboBox;

                Part_Continuity.SelectionChanged -= new SelectionChangedEventHandler(Part_Continuity_SelectionChanged);
                if (SelectedItemIndex == -1)
                {
                    Part_Continuity.SelectedIndex = -1;
                }
                else if (SelectedKeyframeIndex == -1)
                {
                    Part_Continuity.SelectedIndex = -1;
                }
                else if (SelectedKeyframeIndex >= (Items[SelectedItemIndex] as AnimationChannel).curve1.Keys.Count)
                {
                    Part_Continuity.SelectedIndex = -1;
                }
                else
                {
                    int si = SelectedItemIndex;
                    int ki = SelectedKeyframeIndex;
                    AnimationChannel tweenable = Items[si] as AnimationChannel;
                    Part_Continuity.SelectedIndex = (int)tweenable.curve1.Keys[ki].Continuity;
                }
                Part_Continuity.SelectionChanged += new SelectionChangedEventHandler(Part_Continuity_SelectionChanged);
                */
            }
        }

        public static RoutedUICommand SelectColor
        {
            get;
            private set;
        }

        public IList<object> VisibleItems
        {
            get
            {
                return visibleItems;
            }
        }

        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();

            /*
            DataGrid Part_AnimationChannels = Template.FindName("Part_AnimationChannels", this) as DataGrid;
            if (Part_AnimationChannels == null) return;
            Part_AnimationChannels.ApplyTemplate();

            ScrollViewer viewer = Part_AnimationChannels.Template.FindName("Part_ScrollViewer", Part_AnimationChannels) as ScrollViewer;
            if (viewer == null) return;
            viewer.ApplyTemplate();

            ComboBox Part_Easing = viewer.Template.FindName("Part_Easing", viewer) as ComboBox;
            if (Part_Easing == null) return;
            Part_Easing.SelectionChanged += new SelectionChangedEventHandler(Part_Easing_SelectionChanged);

            ComboBox Part_Continuity = viewer.Template.FindName("Part_Continuity", viewer) as ComboBox;
            if (Part_Continuity == null) return;
            Part_Continuity.SelectionChanged += new SelectionChangedEventHandler(Part_Continuity_SelectionChanged);
            Part_AnimationChannels.SelectionChanged += new SelectionChangedEventHandler(Part_AnimationChannels_SelectionChanged);
            */
        }

        public void NotifyCurveInvalidated(AnimationChannel Tweenable)
        {
            if (Tweenable != null)
            {
                Tweenable.Value = Tweenable.curve1.Evaluate((float)AnimationCurrentTime);
                if (OnTimeChanged != null) OnTimeChanged.Invoke(this, EventArgs.Empty);
            }

            /*
            DataGrid Part_AnimationChannels = Template.FindName("Part_AnimationChannels", this) as DataGrid;
            if (Part_AnimationChannels == null) return;
            ScrollViewer viewer = Part_AnimationChannels.Template.FindName("Part_ScrollViewer", Part_AnimationChannels) as ScrollViewer;
            if (viewer == null) return;
            AnimationCurvePreview splineEditor = viewer.Template.FindName("Part_Preview", viewer) as AnimationCurvePreview;
            if (splineEditor == null) return;
            splineEditor.InvalidateVisual();
            */
        }

        public void NotifyValueIncrement(double value)
        {
            /*
            int selectedKey = -1;
            DataGrid Part_AnimationChannels = Template.FindName("Part_AnimationChannels", this) as DataGrid;
            AnimationChannel channel = Items[Part_AnimationChannels.SelectedIndex] as AnimationChannel;


            double pos = Single.PositiveInfinity; double offset = AnimationCurrentTime;
            for (int i = 0; i < channel.curve1.Keys.Count; i++)
            {
                double distance = Math.Abs(channel.curve1.Keys[i].Position - offset);
                if (distance < pos)
                {
                    pos = distance;
                    selectedKey = i;
                }
            }

            if (selectedKey > -1)
            {
                CurveKey keya = channel.curve1.Keys[selectedKey];
                float val = keya.Value + (float)value;
                if (val > channel.Maximum) val = channel.Maximum;
                if (val < channel.Minimum) val = channel.Minimum;

                channel.curve1.Keys.RemoveAt(selectedKey);
                channel.curve1.Keys.Add(new CurveKey(keya.Position, val, keya.TangentIn, keya.TangentOut, keya.Continuity));
                channel.curve1.ComputeTangents(channel.Easying);
                NotifyCurveInvalidated(channel);
            }
            */
        }

        #endregion

        #region Protected Members


        protected override void OnItemsSourceChanged(System.Collections.IEnumerable oldValue, System.Collections.IEnumerable newValue)
        {
            visibleItems.Clear();
            if (oldValue != null) foreach (object c in oldValue)
                {
                    if ((c is System.ComponentModel.INotifyPropertyChanged)) (c as System.ComponentModel.INotifyPropertyChanged).PropertyChanged -= new System.ComponentModel.PropertyChangedEventHandler(AnimationEditor_PropertyChanged);
                    if (IsVisible(c))
                    {
                        visibleItems.Remove(c);
                    }
                }

            if (newValue != null) foreach (object c in newValue)
                {
                    if ((c is AnimationChannel)) (c as AnimationChannel).Value = (c as AnimationChannel).curve1.Evaluate((float)AnimationCurrentTime);
                    if ((c is System.ComponentModel.INotifyPropertyChanged)) (c as System.ComponentModel.INotifyPropertyChanged).PropertyChanged += new System.ComponentModel.PropertyChangedEventHandler(AnimationEditor_PropertyChanged);
                    if (IsVisible(c))
                    {
                        visibleItems.Add(c);
                    }
                }

            base.OnItemsSourceChanged(oldValue, newValue);
        }

        protected override void OnItemsChanged(System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == System.Collections.Specialized.NotifyCollectionChangedAction.Reset)
            {
                List<object> removableItems = new List<object>();
                foreach (object c in visibleItems)
                {
                    if (Items.Contains(c) == false)
                    {
                        removableItems.Add(c);
                    }
                }

                foreach (object c in removableItems)
                {
                    if ((c is System.ComponentModel.INotifyPropertyChanged)) (c as System.ComponentModel.INotifyPropertyChanged).PropertyChanged -= new System.ComponentModel.PropertyChangedEventHandler(AnimationEditor_PropertyChanged);
                    if (visibleItems.Contains(c))
                        visibleItems.Remove(c);
                }

                foreach (object c in Items)
                {
                    if ((c is AnimationChannel)) (c as AnimationChannel).Value = (c as AnimationChannel).curve1.Evaluate((float)AnimationCurrentTime);
                    if ((c is System.ComponentModel.INotifyPropertyChanged)) (c as System.ComponentModel.INotifyPropertyChanged).PropertyChanged += new System.ComponentModel.PropertyChangedEventHandler(AnimationEditor_PropertyChanged);
                    if (IsVisible(c))
                    {
                        visibleItems.Add(c);
                    }
                }


            }




            if (e.OldItems != null) foreach (object c in e.OldItems)
                {
                    if ((c is System.ComponentModel.INotifyPropertyChanged)) (c as System.ComponentModel.INotifyPropertyChanged).PropertyChanged -= new System.ComponentModel.PropertyChangedEventHandler(AnimationEditor_PropertyChanged);
                    if (visibleItems.Contains(c))
                        visibleItems.Remove(c);
                }

            if (e.NewItems != null) foreach (object c in e.NewItems)
                {
                    if ((c is AnimationChannel)) (c as AnimationChannel).Value = (c as AnimationChannel).curve1.Evaluate((float)AnimationCurrentTime);
                    if ((c is System.ComponentModel.INotifyPropertyChanged)) (c as System.ComponentModel.INotifyPropertyChanged).PropertyChanged += new System.ComponentModel.PropertyChangedEventHandler(AnimationEditor_PropertyChanged);
                    if (IsVisible(c))
                    {
                        visibleItems.Add(c);
                    }
                }

            base.OnItemsChanged(e);


            NotifyCurveInvalidated(null);

        }

        protected bool IsVisible(object c)
        {
            AnimationChannel tween = c as AnimationChannel;
            return tween.Visible;
        }

        #endregion

        #region Ctor / Dtor

        static AnimationEditor()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(AnimationEditor), new FrameworkPropertyMetadata(typeof(AnimationEditor)));
            SelectColor = new RoutedUICommand("Select Color", "SelectColor", typeof(AnimationEditor));

        }

        public AnimationEditor()
        {
            CommandBindings.Add(new CommandBinding(SelectColor, new ExecutedRoutedEventHandler(RoutedCommand_SelectColor_Executed)));
        }

        #endregion

    }

    public delegate void TimeChangedEventHandler(object o, EventArgs e);

    public class AnimationKeyFrameRuler : Control
    {
        #region Private Members

        private bool istracing = false;
        private const int GeometrySize = 4, GeometryHitTestSize = 7;

        private static void OnKeyFramesChanged(DependencyObject o, DependencyPropertyChangedEventArgs e)
        {
            (o as AnimationKeyFrameRuler).InvalidateVisual();
        }



        #endregion

        #region Public Members


        public static readonly DependencyProperty SourceProperty = DependencyProperty.Register("Source",
            typeof(AnimationChannel), typeof(AnimationKeyFrameRuler), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.AffectsRender, new PropertyChangedCallback(OnKeyFramesChanged)));
        public static readonly DependencyProperty SelectedItemIndexProperty = DependencyProperty.Register("SelectedItemIndex",
            typeof(int), typeof(AnimationKeyFrameRuler), new FrameworkPropertyMetadata(0, FrameworkPropertyMetadataOptions.AffectsRender, null));
        public static readonly DependencyProperty SelectedIndexProperty = DependencyProperty.Register("SelectedIndex",
            typeof(int), typeof(AnimationKeyFrameRuler), new FrameworkPropertyMetadata(0, FrameworkPropertyMetadataOptions.AffectsRender, null));
        public static readonly DependencyProperty OffsetProperty = DependencyProperty.Register("Offset",
            typeof(float), typeof(AnimationKeyFrameRuler), new FrameworkPropertyMetadata(0.0f, FrameworkPropertyMetadataOptions.AffectsRender));
        public static readonly DependencyProperty ScaleProperty = DependencyProperty.Register("Scale",
            typeof(float), typeof(AnimationKeyFrameRuler), new FrameworkPropertyMetadata(100.0f, FrameworkPropertyMetadataOptions.AffectsRender | FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));

        public AnimationChannel Source
        {
            get
            {
                return (AnimationChannel)this.GetValue(SourceProperty);
            }
            set
            {
                this.SetValue(SourceProperty, value);
            }
        }

        public int SelectedItemIndex
        {
            get
            {
                return (int)this.GetValue(SelectedItemIndexProperty);
            }
            set
            {
                this.SetValue(SelectedItemIndexProperty, value);
            }
        }

        public int SelectedIndex
        {
            get
            {
                return (int)this.GetValue(SelectedIndexProperty);
            }
            set
            {
                this.SetValue(SelectedIndexProperty, value);
            }
        }

        public float Offset
        {
            get
            {
                return (float)this.GetValue(OffsetProperty);
            }
            set
            {
                this.SetValue(OffsetProperty, value);
            }
        }

        public float Scale
        {
            get
            {
                return (float)this.GetValue(ScaleProperty);
            }
            set
            {
                this.SetValue(ScaleProperty, value);
            }
        }

        #endregion

        #region Protected Members

        protected override void OnRender(DrawingContext drawingContext)
        {



            AnimationEditor grid = UIHelper.TryFindParent<AnimationEditor>(this);
            bool selected = grid.SelectedItemIndex == grid.Items.IndexOf(Source);
            float scale = Scale;





            StreamGeometry geometry = new StreamGeometry() { FillRule = FillRule.EvenOdd };
            using (StreamGeometryContext ctx = geometry.Open())
            {
                ctx.BeginFigure(new System.Windows.Point(0, -GeometrySize), true, true);
                ctx.LineTo(new System.Windows.Point(GeometrySize, 0), false, false);
                ctx.LineTo(new System.Windows.Point(0, GeometrySize), false, false);
                ctx.LineTo(new System.Windows.Point(-GeometrySize, 0), false, false);
                ctx.LineTo(new System.Windows.Point(0, -GeometrySize), false, false);
                ctx.Close();
            }


            drawingContext.DrawGeometry(Brushes.Transparent, null,
                new RectangleGeometry(new Rect(0, 0, ActualWidth, ActualHeight), 0, 0));
            if (Source != null)
            {
                int selIndex = SelectedIndex;
                drawingContext.PushClip(GetLayoutClip(new Size(ActualWidth, ActualHeight)));
                drawingContext.PushTransform(new TranslateTransform(-Offset, 0));
                for (int i = 0; i < Source.curve1.Keys.Count; i++)
                {
                    CurveKey key = Source.curve1.Keys[i];
                    drawingContext.PushTransform(new TranslateTransform(key.Position / 100.0f * scale, (ActualHeight / 2)));
                    drawingContext.DrawGeometry(i == grid.SelectedKeyframeIndex && selected ? Brushes.Orange : Brushes.Black, null, geometry);
                    drawingContext.Pop();
                }

                drawingContext.Pop();
                drawingContext.Pop();
            }














        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            if (Source != null)
            {
                int sIndex = -1;
                float d = Offset + (float)e.GetPosition(this).X * 100 / Scale;
                for (int i = 0; i < Source.curve1.Keys.Count; i++)
                {
                    CurveKey key = Source.curve1.Keys[i];
                    if (Math.Abs(key.Position - d) < GeometryHitTestSize)
                    {
                        sIndex = i; break;
                    }
                }

                SelectedIndex = sIndex;
                AnimationEditor grid = UIHelper.TryFindParent<AnimationEditor>(this);
                grid.SelectedItemIndex = grid.Items.IndexOf(Source);
                grid.SelectedKeyframeIndex = SelectedIndex;
            }

            if (SelectedIndex > -1)
            {


                //Notify the curve needs to be redrawn...
                AnimationEditor editor = UIHelper.TryFindParent<AnimationEditor>(this);
                if (editor != null)
                {
                    editor.AnimationCurrentTime = Source.curve1.Keys[SelectedIndex].Position;
                }

                istracing = Mouse.Capture(this);
            }
        }

        protected override void OnMouseRightButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseRightButtonDown(e);

            if (Source != null)
            {
                int sIndex = -1;
                float d = Offset + (float)e.GetPosition(this).X * 100 / Scale;
                for (int i = 0; i < Source.curve1.Keys.Count; i++)
                {
                    CurveKey key = Source.curve1.Keys[i];
                    if (Math.Abs(key.Position - d) < GeometryHitTestSize)
                    {
                        sIndex = i; break;
                    }
                }

                SelectedIndex = sIndex;
                AnimationEditor grid = UIHelper.TryFindParent<AnimationEditor>(this);
                grid.SelectedItemIndex = grid.Items.IndexOf(Source);
                grid.SelectedKeyframeIndex = SelectedIndex;
            }

            if (SelectedIndex == -1)
            {
                if (Source != null)
                {
                    float d = Offset + (float)e.GetPosition(this).X * 100 / Scale;
                    Source.curve1.Keys.Add(new CurveKey(d, Source.curve1.Evaluate(d), 0, 0, CurveContinuity.Smooth));
                    Source.curve2.Keys.Add(new CurveKey(d, d, 0, 0, CurveContinuity.Smooth));
                    Source.curve1.ComputeTangents(Source.Easying);
                    Source.curve2.ComputeTangents(Source.Easying);
                    InvalidateVisual();

                    //Notify the curve needs to be redrawn...
                    AnimationEditor editor = UIHelper.TryFindParent<AnimationEditor>(this);
                    if (editor != null)
                    {
                        editor.NotifyCurveInvalidated(Source);
                    }
                }
            }
            else
            {
                if (Source != null)
                {
                    Source.curve1.Keys.RemoveAt(SelectedIndex);
                    Source.curve2.Keys.RemoveAt(SelectedIndex);
                    Source.curve1.ComputeTangents(Source.Easying);
                    Source.curve2.ComputeTangents(Source.Easying);
                    InvalidateVisual();
                    SelectedIndex = -1;

                    //Notify the curve needs to be redrawn...
                    AnimationEditor editor = UIHelper.TryFindParent<AnimationEditor>(this);
                    if (editor != null)
                    {
                        editor.NotifyCurveInvalidated(Source);
                    }
                }
            }
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            if (istracing == true)
            {
                double pos = Offset + (float)e.GetPosition(this).X * 100 / Scale;
                if (pos < 0)
                {
                    pos = 0;
                }


                if (SelectedIndex > 0)
                {
                    if (pos <= Source.curve1.Keys[SelectedIndex - 1].Position)
                        return;
                }

                if (SelectedIndex < Source.curve1.Keys.Count - 1)
                {
                    if (pos >= Source.curve1.Keys[SelectedIndex + 1].Position)
                        return;
                }



                CurveKey keya = Source.curve1.Keys[SelectedIndex];
                CurveKey keyb = Source.curve2.Keys[SelectedIndex];
                Source.curve1.Keys.RemoveAt(SelectedIndex);
                Source.curve2.Keys.RemoveAt(SelectedIndex);
                Source.curve1.Keys.Add(new CurveKey((float)pos, keya.Value, keya.TangentIn, keya.TangentOut, keya.Continuity));
                Source.curve2.Keys.Add(new CurveKey((float)pos, (float)pos, keyb.TangentIn, keyb.TangentOut, keyb.Continuity));
                Source.curve1.ComputeTangents(Source.Easying);
                Source.curve2.ComputeTangents(Source.Easying);
                InvalidateVisual();


                //Notify the curve needs to be redrawn...
                AnimationEditor editor = UIHelper.TryFindParent<AnimationEditor>(this);
                if (editor != null)
                {
                    editor.NotifyCurveInvalidated(Source);
                }
            }
        }

        protected override void OnMouseLeftButtonUp(MouseButtonEventArgs e)
        {
            istracing = false;
            Mouse.Capture(this, CaptureMode.None);
            base.OnMouseLeftButtonUp(e);
        }

        #endregion

        #region Ctor / Dtor

        static AnimationKeyFrameRuler()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(AnimationKeyFrameRuler), new FrameworkPropertyMetadata(typeof(AnimationKeyFrameRuler)));
        }

        #endregion
    }

    /*
    public class AnimationLengthConverter : IValueConverter
    {
        #region IValueConverter Members

        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return (float)value * 100;
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }

        #endregion
    }

    public class ColorToBrushConverter : IValueConverter
    {
        #region IValueConverter Members

        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            SolidColorBrush brush = new SolidColorBrush((Color)value);
            return brush;
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }

        #endregion
    }

    public class EasyingToIndexConverter : IValueConverter
    {
        #region IValueConverter Members

        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return (int)((CurveTangent)value);
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return (CurveTangent)((int)value);
        }

        #endregion
    }

    public class TimeEvaluatedValueConverter : IValueConverter
    {
        #region IValueConverter Members

        object IValueConverter.Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            double animTime = (parameter as AnimationEditor).AnimationCurrentTime;
            return (value as AnimationChannel).curve1.Evaluate((float)animTime);
        }

        object IValueConverter.ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }

        #endregion
    }
    */
}
