using System;
using Android.App;
using Android.Content;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.OS;
using Android.Bluetooth;
using Java.Lang;


namespace IrrigationController
{
    [Activity(Label = "IrrigationController", MainLauncher = true, Icon = "@drawable/icon")]

    public class BTReceiver: BroadcastReceiver
    {
        private ArrayAdapter<string> m_arrayBT = null;
        private bool m_bDiscoveryComplete = false;

        public BTReceiver(ArrayAdapter<string> arrayBT)
        {
            m_arrayBT = arrayBT;
        }

        public override void OnReceive(Context context, Intent intent)
        {
            string strAction = intent.Action;

            // When discovery finds a device
            if (strAction == BluetoothDevice.ActionFound)
            {
                // Get the BluetoothDevice object from the Intent
                BluetoothDevice device = (BluetoothDevice)intent.GetParcelableExtra(BluetoothDevice.ExtraDevice);

                // If it's already paired, skip it, because it's been listed already
                if (device.Name.Contains("Irrigation") || device.Name.Contains("irrigation"))
                {
                    m_arrayBT.Add(device.Name + "\n" + device.Address);
                }
                // When discovery is finished, change the Activity title
            }
            else if (strAction == BluetoothAdapter.ActionDiscoveryFinished)
            {
                m_bDiscoveryComplete = true;
            }
        }

        public bool IsDicoveryComplete()
        {
            return m_bDiscoveryComplete;
        }

        public bool DevicesFound()
        {
            return m_arrayBT.Count > 0;
        }
    }

    public class MainActivity : Activity
    {
        private BluetoothAdapter m_BluetoothAdapter = BluetoothAdapter.DefaultAdapter;
        private TextView m_textview = null;
        private static int REQUEST_ENABLE_BT = 1;
        private ArrayAdapter<string> m_arrayBT = null;
        private BTReceiver m_BTReceiver;
        private AlertDialog m_dlgMsg = null;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            Android.App.AlertDialog.Builder builder = new AlertDialog.Builder(this);
            m_dlgMsg = builder.Create();

//            Spinner spinnerBTDevices = FindViewById<Spinner>(Resource.Id.spinnerBTDevices);
            m_arrayBT = new ArrayAdapter<string>(this, Resource.Layout.SpinnerItem);
//            spinnerBTDevices.Adapter = m_arrayBT;

            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.Main);

//            m_textview = FindViewById<TextView>(Resource.Id.textviewStatus);

            if (m_BluetoothAdapter != null)
                m_textview.SetText(Resource.String.BTUnavailable);
            else if (!m_BluetoothAdapter.IsEnabled)
            {
                //textView.SetText(Resource.String.BTDisabled);
                var EnableBTIntent = new Intent(BluetoothAdapter.ActionRequestEnable);
                StartActivityForResult(EnableBTIntent, REQUEST_ENABLE_BT);
            }
            else
                m_textview.SetText(Resource.String.NotConnected);
        }

        protected void RegisterBTReceivers()
        {
            // Register for broadcasts when a device is discovered
            m_BTReceiver = new BTReceiver(m_arrayBT);
            var filter = new IntentFilter(BluetoothDevice.ActionFound);
            RegisterReceiver(m_BTReceiver, filter);

            // Register for broadcasts when discovery has finished
            filter = new IntentFilter(BluetoothAdapter.ActionDiscoveryFinished);
            RegisterReceiver(m_BTReceiver, filter);
        }

        protected void StartDiscovery()
        {
            RegisterBTReceivers();

            if (m_BluetoothAdapter.IsDiscovering)
                m_BluetoothAdapter.CancelDiscovery();

            // Request discover from BluetoothAdapter
            m_BluetoothAdapter.StartDiscovery();
        }

        private void WaitDiscoveryComplete()
        {
            Int64 nStartMillis = JavaSystem.CurrentTimeMillis(), nDeltaMillis = 0;

            while (!m_BTReceiver.IsDicoveryComplete() && (nDeltaMillis < 5000))
            {
                nDeltaMillis = JavaSystem.CurrentTimeMillis() - nStartMillis;
            }
        }

        private bool CheckForBTEnable()
        {
            Int64 nStartMillis = JavaSystem.CurrentTimeMillis(), nDeltaMillis = 0;
            
            while (!m_BluetoothAdapter.IsEnabled && (nDeltaMillis < 2000))
            {
                nDeltaMillis = JavaSystem.CurrentTimeMillis() - nStartMillis;
            }
            return m_BluetoothAdapter.IsEnabled;
        }

        protected void DisplayMessage(string strTitle, string strMsg)
        {
            m_dlgMsg.SetTitle(strTitle);
            m_dlgMsg.SetMessage(strMsg);
            m_dlgMsg.SetButton("OK", (s, ev) => { });
            m_dlgMsg.SetIcon(Android.Resource.Drawable.IcDialogAlert);
            m_dlgMsg.Show();
        }

        protected override void OnStart()
        {
            base.OnStart();
DisplayMessage("DEBUG", "In OnStart() before");
            if (CheckForBTEnable())
            {
DisplayMessage("DEBUG", "In OnStart() after");
//                Button buttonRefresh = FindViewById<Button>(Resource.Id.buttonRefresh);
//                Button buttonConnect = FindViewById<Button>(Resource.Id.buttonConnect);
                StartDiscovery();
                WaitDiscoveryComplete();
                if (m_BTReceiver.DevicesFound())
                {

                }
            }
        }

        protected override void OnActivityResult(Int32 requestCode, Result resultCode, Intent data)
        {
            if (requestCode == REQUEST_ENABLE_BT)
            {
                if (m_BluetoothAdapter.IsEnabled)
                    m_textview.SetText(Resource.String.NotConnected);
                else
                    m_textview.SetText(Resource.String.BTDisabled);
            }
        }
    }
}

