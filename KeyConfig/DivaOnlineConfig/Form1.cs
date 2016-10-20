using MiniJSON;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace DivaOnlineConfig
{
	public class Form1 : Form
	{
		private enum FormState
		{
			Normal,
			KeyInput
		}
		private const int MAXKEY = 10;
		private List<Keys>[] keyMap;
		private Form1.FormState state;
		private int selectedKey;
		private List<Keys> tempKeys;
		private string ConfigPath = "config/key.json";
		private IContainer components;
		private TabControl tabControl1;
		private TabPage tabPage1;
		private TabControl tabControl2;
		private TabPage tabPage2;
		private TextBox textBoxS;
		private Button buttonS;
		private Button buttonD;
		private Button buttonA;
		private TextBox textBoxA;
		private TextBox textBoxD;
		private TextBox textBoxW;
		private Button buttonW;
		private Button button1;
		private TextBox textBoxDown;
		private TextBox textBoxLeft;
		private TextBox textBoxRight;
		private TextBox textBoxUp;
		private Button buttonDown;
		private Button buttonRight;
		private Button buttonLeft;
		private Button buttonUp;
		private Button button2;
		private Button button3;
		private Button button4;
		private Label label1;
		private ToolTip toolTipKeys;
		private TextBox textBoxCombo;
		private Button buttonCombo;
		private TextBox textBoxRelay;
		private Button buttonRelay;
		private TextBox textBoxCT;
        private PictureBox pictureBox1;
        private Button buttonCT;

		public Form1()
		{
			this.InitializeComponent();
			this.state = Form1.FormState.Normal;
			this.selectedKey = -1;
			this.keyMap = new List<Keys>[10];
			for (int i = 0; i < 10; i++)
			{
				this.keyMap[i] = new List<Keys>();
			}
			this.tempKeys = new List<Keys>();
			this.loadConfig(this.ConfigPath);
		}

		private void setDefaultKeyMap()
		{
			for (int i = 0; i < 10; i++)
			{
				this.keyMap[i].Clear();
				this.keyMap[i].Add(this.getDefaultKeyCode(i));
			}
		}

		private Keys getDefaultKeyCode(int id)
		{
			if (id == 0)
			{
				return Keys.W;
			}
			if (id == 1)
			{
				return Keys.S;
			}
			if (id == 2)
			{
				return Keys.A;
			}
			if (id == 3)
			{
				return Keys.D;
			}
			if (id == 4)
			{
				return Keys.Up;
			}
			if (id == 5)
			{
				return Keys.Down;
			}
			if (id == 6)
			{
				return Keys.Left;
			}
			if (id == 7)
			{
				return Keys.Right;
			}
			if (id == 8)
			{
				return Keys.ShiftKey;
			}
			if (id == 9)
			{
				return Keys.Space;
			}
			throw new IndexOutOfRangeException();
		}

		private Button getButton(int id)
		{
			if (id == 0)
			{
				return this.buttonW;
			}
			if (id == 1)
			{
				return this.buttonS;
			}
			if (id == 2)
			{
				return this.buttonA;
			}
			if (id == 3)
			{
				return this.buttonD;
			}
			if (id == 4)
			{
				return this.buttonUp;
			}
			if (id == 5)
			{
				return this.buttonDown;
			}
			if (id == 6)
			{
				return this.buttonLeft;
			}
			if (id == 7)
			{
				return this.buttonRight;
			}
			if (id == 8)
			{
				return this.buttonCT;
			}
			if (id == 9)
			{
				return this.buttonCombo;
			}
			throw new IndexOutOfRangeException();
		}

		private TextBox getTextBox(int id)
		{
			if (id == 0)
			{
				return this.textBoxW;
			}
			if (id == 1)
			{
				return this.textBoxS;
			}
			if (id == 2)
			{
				return this.textBoxA;
			}
			if (id == 3)
			{
				return this.textBoxD;
			}
			if (id == 4)
			{
				return this.textBoxUp;
			}
			if (id == 5)
			{
				return this.textBoxDown;
			}
			if (id == 6)
			{
				return this.textBoxLeft;
			}
			if (id == 7)
			{
				return this.textBoxRight;
			}
			if (id == 8)
			{
				return this.textBoxCT;
			}
			if (id == 9)
			{
				return this.textBoxCombo;
			}
			throw new IndexOutOfRangeException();
		}

		private int getWidgetId(object w)
		{
			if (w == this.buttonW || w == this.textBoxW)
			{
				return 0;
			}
			if (w == this.buttonS || w == this.textBoxS)
			{
				return 1;
			}
			if (w == this.buttonA || w == this.textBoxA)
			{
				return 2;
			}
			if (w == this.buttonD || w == this.textBoxD)
			{
				return 3;
			}
			if (w == this.buttonUp || w == this.textBoxUp)
			{
				return 4;
			}
			if (w == this.buttonDown || w == this.textBoxDown)
			{
				return 5;
			}
			if (w == this.buttonLeft || w == this.textBoxLeft)
			{
				return 6;
			}
			if (w == this.buttonRight || w == this.textBoxRight)
			{
				return 7;
			}
			if (w == this.buttonCT || w == this.textBoxCT)
			{
				return 8;
			}
			if (w == this.buttonCombo || w == this.textBoxCombo)
			{
				return 9;
			}
			throw new IndexOutOfRangeException();
		}

		private void stopKeyInput()
		{
			this.state = Form1.FormState.Normal;
			if (this.tempKeys.Count != 0)
			{
				for (int i = 0; i < 10; i++)
				{
					for (int j = this.keyMap[i].Count - 1; j >= 0; j--)
					{
						if (this.tempKeys.Contains(this.keyMap[i][j]))
						{
							this.keyMap[i].RemoveAt(j);
						}
					}
				}
				this.keyMap[this.selectedKey].Clear();
				foreach (Keys current in this.tempKeys)
				{
					this.keyMap[this.selectedKey].Add(current);
				}
				this.tempKeys.Clear();
			}
			this.refreshKeyMap();
		}

		private void keyButton_Click(object sender, EventArgs e)
		{
			if (this.state != Form1.FormState.Normal)
			{
				this.stopKeyInput();
				if (this.selectedKey == this.getWidgetId(sender))
				{
					return;
				}
			}
			int widgetId = this.getWidgetId(sender);
			this.tempKeys.Clear();
			this.selectedKey = widgetId;
			this.state = Form1.FormState.KeyInput;
			this.refreshKeyMap();
			this.getTextBox(widgetId).Focus();
		}

		private string keysToString(List<Keys> keys)
		{
			string text = "";
			foreach (Keys current in keys)
			{
				text = text + "," + current.ToString();
			}
			return text.Substring(1);
		}

		private void refreshKeyMap()
		{
			for (int i = 0; i < 10; i++)
			{
				if (this.state == Form1.FormState.KeyInput && this.selectedKey == i)
				{
					if (this.tempKeys.Count == 0)
					{
						this.getTextBox(i).Text = "Waiting...";
						this.getTextBox(i).ForeColor = Color.Green;
					}
					else
					{
						this.getTextBox(i).Text = "Key Pressed: " + this.keysToString(this.tempKeys);
						this.getTextBox(i).ForeColor = Color.Green;
					}
				}
				else if (this.keyMap[i].Count == 0)
				{
					this.getTextBox(i).Text = "Not Set";
					this.getTextBox(i).ForeColor = Color.Red;
				}
				else
				{
					this.getTextBox(i).Text = this.keysToString(this.keyMap[i]);
					this.getTextBox(i).ForeColor = Color.Black;
				}
			}
		}

		private void keyTextBox_MouseEnter(object sender, EventArgs e)
		{
		}

		private void button3_Click(object sender, EventArgs e)
		{
			base.Close();
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			this.refreshKeyMap();
		}

		private void Form1_KeyDown(object sender, KeyEventArgs e)
		{
		}

		private void keyTextBox_KeyDown(object sender, KeyEventArgs e)
		{
			if (this.state == Form1.FormState.Normal)
			{
				return;
			}
			if (e.KeyCode == Keys.Escape)
			{
				this.stopKeyInput();
			}
			else if (!this.tempKeys.Contains(e.KeyCode))
			{
				this.tempKeys.Add(e.KeyCode);
			}
			this.refreshKeyMap();
		}

		private void textBoxDown_TextChanged(object sender, EventArgs e)
		{
		}

		private void textBoxUp_TextChanged(object sender, EventArgs e)
		{
		}

		private void textBoxS_TextChanged(object sender, EventArgs e)
		{
		}

		private void textBoxW_TextChanged(object sender, EventArgs e)
		{
		}

		private void button1_Click(object sender, EventArgs e)
		{
			if (MessageBox.Show("Reset settings to default values?", "", MessageBoxButtons.YesNo) == DialogResult.Yes)
			{
				this.setDefaultKeyMap();
				this.refreshKeyMap();
			}
		}

		private void textBoxD_MouseHover(object sender, EventArgs e)
		{
			this.toolTipKeys.Show(((TextBox)sender).Text, (TextBox)sender);
		}

		private void saveConfig(string filename)
		{
            try
            {
			    using (StreamWriter streamWriter = new StreamWriter(filename))
			    {
				    Dictionary<string, object> dictionary = new Dictionary<string, object>();
				    string[] keyNames = this.getKeyNames();
				    for (int i = 0; i < 10; i++)
				    {
					    dictionary.Add(keyNames[i], new List<object>());
					    this.setDict(this.keyMap[i], dictionary[keyNames[i]] as List<object>);
				    }
				    dictionary.Add("DIVA_KEY_REPLY", new List<object>());
				    ((List<object>)dictionary["DIVA_KEY_REPLY"]).Add(9);
				    streamWriter.Write(Json.Serialize(dictionary));
			    }
            }
            catch(Exception)
            {
                MessageBox.Show("Unable to save configuration, does the file 'config/key.json' exist?");
            }
		}

		private void setDict(List<Keys> map, List<object> dict)
		{
			dict.Clear();
			foreach (Keys current in map)
			{
				dict.Add((int)current);
			}
		}

		private void setKeyMap(List<Keys> map, List<object> dict)
		{
			map.Clear();
			foreach (object current in dict)
			{
				map.Add((Keys)Convert.ToInt32(current));
			}
		}

		private string[] getKeyNames()
		{
			return new string[]
			{
				"DIVA_KEY_TYPE_3",
				"DIVA_KEY_TYPE_2",
				"DIVA_KEY_TYPE_1",
				"DIVA_KEY_TYPE_0",
				"DIVA_KEY_TYPE_7",
				"DIVA_KEY_TYPE_6",
				"DIVA_KEY_TYPE_5",
				"DIVA_KEY_TYPE_4",
				"DIVA_KEY_CT",
				"DIVA_KEY_SPACE"
			};
		}

		private void loadConfig(string filename)
		{
			if (!File.Exists(filename))
			{
				this.setDefaultKeyMap();
				return;
			}
			try
			{
				using (StreamReader streamReader = new StreamReader(filename))
				{
					Dictionary<string, object> dictionary = Json.Deserialize(streamReader.ReadToEnd()) as Dictionary<string, object>;
					string[] keyNames = this.getKeyNames();
					for (int i = 0; i < 10; i++)
					{
						this.setKeyMap(this.keyMap[i], dictionary[keyNames[i]] as List<object>);
					}
				}
			}
			catch (Exception)
			{
				MessageBox.Show("An error has occured. Using default settings.");
				this.setDefaultKeyMap();
			}
		}

		private void button2_Click(object sender, EventArgs e)
		{
			this.stopKeyInput();
			this.saveConfig(this.ConfigPath);
			base.Close();
		}

		private void button4_Click(object sender, EventArgs e)
		{
			this.stopKeyInput();
			this.saveConfig(this.ConfigPath);
		}

		protected override void Dispose(bool disposing)
		{
			if (disposing && this.components != null)
			{
				this.components.Dispose();
			}
			base.Dispose(disposing);
		}

		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.tabControl2 = new System.Windows.Forms.TabControl();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.textBoxCombo = new System.Windows.Forms.TextBox();
            this.buttonCombo = new System.Windows.Forms.Button();
            this.textBoxRelay = new System.Windows.Forms.TextBox();
            this.buttonRelay = new System.Windows.Forms.Button();
            this.textBoxCT = new System.Windows.Forms.TextBox();
            this.buttonCT = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.textBoxDown = new System.Windows.Forms.TextBox();
            this.textBoxLeft = new System.Windows.Forms.TextBox();
            this.textBoxRight = new System.Windows.Forms.TextBox();
            this.textBoxUp = new System.Windows.Forms.TextBox();
            this.buttonDown = new System.Windows.Forms.Button();
            this.buttonRight = new System.Windows.Forms.Button();
            this.buttonLeft = new System.Windows.Forms.Button();
            this.buttonUp = new System.Windows.Forms.Button();
            this.textBoxS = new System.Windows.Forms.TextBox();
            this.buttonS = new System.Windows.Forms.Button();
            this.buttonD = new System.Windows.Forms.Button();
            this.buttonA = new System.Windows.Forms.Button();
            this.textBoxA = new System.Windows.Forms.TextBox();
            this.textBoxD = new System.Windows.Forms.TextBox();
            this.textBoxW = new System.Windows.Forms.TextBox();
            this.buttonW = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.toolTipKeys = new System.Windows.Forms.ToolTip(this.components);
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabControl2.SuspendLayout();
            this.tabPage2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Location = new System.Drawing.Point(0, 108);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(551, 416);
            this.tabControl1.TabIndex = 1;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.tabControl2);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(543, 390);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Game Settings";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // tabControl2
            // 
            this.tabControl2.Controls.Add(this.tabPage2);
            this.tabControl2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl2.Location = new System.Drawing.Point(3, 3);
            this.tabControl2.Name = "tabControl2";
            this.tabControl2.SelectedIndex = 0;
            this.tabControl2.Size = new System.Drawing.Size(537, 384);
            this.tabControl2.TabIndex = 0;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.textBoxCombo);
            this.tabPage2.Controls.Add(this.buttonCombo);
            this.tabPage2.Controls.Add(this.textBoxRelay);
            this.tabPage2.Controls.Add(this.buttonRelay);
            this.tabPage2.Controls.Add(this.textBoxCT);
            this.tabPage2.Controls.Add(this.buttonCT);
            this.tabPage2.Controls.Add(this.label1);
            this.tabPage2.Controls.Add(this.button1);
            this.tabPage2.Controls.Add(this.textBoxDown);
            this.tabPage2.Controls.Add(this.textBoxLeft);
            this.tabPage2.Controls.Add(this.textBoxRight);
            this.tabPage2.Controls.Add(this.textBoxUp);
            this.tabPage2.Controls.Add(this.buttonDown);
            this.tabPage2.Controls.Add(this.buttonRight);
            this.tabPage2.Controls.Add(this.buttonLeft);
            this.tabPage2.Controls.Add(this.buttonUp);
            this.tabPage2.Controls.Add(this.textBoxS);
            this.tabPage2.Controls.Add(this.buttonS);
            this.tabPage2.Controls.Add(this.buttonD);
            this.tabPage2.Controls.Add(this.buttonA);
            this.tabPage2.Controls.Add(this.textBoxA);
            this.tabPage2.Controls.Add(this.textBoxD);
            this.tabPage2.Controls.Add(this.textBoxW);
            this.tabPage2.Controls.Add(this.buttonW);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(529, 358);
            this.tabPage2.TabIndex = 0;
            this.tabPage2.Text = "Keyboard";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // textBoxCombo
            // 
            this.textBoxCombo.Location = new System.Drawing.Point(107, 269);
            this.textBoxCombo.Name = "textBoxCombo";
            this.textBoxCombo.ReadOnly = true;
            this.textBoxCombo.Size = new System.Drawing.Size(130, 20);
            this.textBoxCombo.TabIndex = 27;
            this.textBoxCombo.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textBoxCombo.KeyDown += new System.Windows.Forms.KeyEventHandler(this.keyTextBox_KeyDown);
            this.textBoxCombo.MouseHover += new System.EventHandler(this.textBoxD_MouseHover);
            // 
            // buttonCombo
            // 
            this.buttonCombo.Location = new System.Drawing.Point(15, 268);
            this.buttonCombo.Name = "buttonCombo";
            this.buttonCombo.Size = new System.Drawing.Size(86, 23);
            this.buttonCombo.TabIndex = 26;
            this.buttonCombo.Text = "Even Hit?";
            this.buttonCombo.UseVisualStyleBackColor = true;
            this.buttonCombo.Click += new System.EventHandler(this.keyButton_Click);
            // 
            // textBoxRelay
            // 
            this.textBoxRelay.Location = new System.Drawing.Point(358, 239);
            this.textBoxRelay.Name = "textBoxRelay";
            this.textBoxRelay.ReadOnly = true;
            this.textBoxRelay.Size = new System.Drawing.Size(130, 20);
            this.textBoxRelay.TabIndex = 25;
            this.textBoxRelay.Text = "Tab";
            this.textBoxRelay.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // buttonRelay
            // 
            this.buttonRelay.Enabled = false;
            this.buttonRelay.Location = new System.Drawing.Point(266, 238);
            this.buttonRelay.Name = "buttonRelay";
            this.buttonRelay.Size = new System.Drawing.Size(86, 23);
            this.buttonRelay.TabIndex = 24;
            this.buttonRelay.Text = "Relay";
            this.buttonRelay.UseVisualStyleBackColor = true;
            // 
            // textBoxCT
            // 
            this.textBoxCT.Location = new System.Drawing.Point(107, 239);
            this.textBoxCT.Name = "textBoxCT";
            this.textBoxCT.ReadOnly = true;
            this.textBoxCT.Size = new System.Drawing.Size(130, 20);
            this.textBoxCT.TabIndex = 23;
            this.textBoxCT.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textBoxCT.KeyDown += new System.Windows.Forms.KeyEventHandler(this.keyTextBox_KeyDown);
            this.textBoxCT.MouseHover += new System.EventHandler(this.textBoxD_MouseHover);
            // 
            // buttonCT
            // 
            this.buttonCT.Location = new System.Drawing.Point(15, 238);
            this.buttonCT.Name = "buttonCT";
            this.buttonCT.Size = new System.Drawing.Size(86, 23);
            this.buttonCT.TabIndex = 22;
            this.buttonCT.Text = "Burst";
            this.buttonCT.UseVisualStyleBackColor = true;
            this.buttonCT.Click += new System.EventHandler(this.keyButton_Click);
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 339);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(147, 13);
            this.label1.TabIndex = 21;
            this.label1.Text = "Press ESC to cancel keybind.";
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.button1.Location = new System.Drawing.Point(412, 322);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(111, 30);
            this.button1.TabIndex = 20;
            this.button1.Text = "Default Settings";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // textBoxDown
            // 
            this.textBoxDown.Location = new System.Drawing.Point(296, 186);
            this.textBoxDown.Name = "textBoxDown";
            this.textBoxDown.ReadOnly = true;
            this.textBoxDown.Size = new System.Drawing.Size(130, 20);
            this.textBoxDown.TabIndex = 19;
            this.textBoxDown.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textBoxDown.TextChanged += new System.EventHandler(this.textBoxDown_TextChanged);
            this.textBoxDown.KeyDown += new System.Windows.Forms.KeyEventHandler(this.keyTextBox_KeyDown);
            this.textBoxDown.MouseEnter += new System.EventHandler(this.keyTextBox_MouseEnter);
            this.textBoxDown.MouseHover += new System.EventHandler(this.textBoxD_MouseHover);
            // 
            // textBoxLeft
            // 
            this.textBoxLeft.Location = new System.Drawing.Point(221, 157);
            this.textBoxLeft.Name = "textBoxLeft";
            this.textBoxLeft.ReadOnly = true;
            this.textBoxLeft.Size = new System.Drawing.Size(130, 20);
            this.textBoxLeft.TabIndex = 18;
            this.textBoxLeft.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textBoxLeft.KeyDown += new System.Windows.Forms.KeyEventHandler(this.keyTextBox_KeyDown);
            this.textBoxLeft.MouseEnter += new System.EventHandler(this.keyTextBox_MouseEnter);
            this.textBoxLeft.MouseHover += new System.EventHandler(this.textBoxD_MouseHover);
            // 
            // textBoxRight
            // 
            this.textBoxRight.Location = new System.Drawing.Point(369, 157);
            this.textBoxRight.Name = "textBoxRight";
            this.textBoxRight.ReadOnly = true;
            this.textBoxRight.Size = new System.Drawing.Size(130, 20);
            this.textBoxRight.TabIndex = 17;
            this.textBoxRight.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textBoxRight.KeyDown += new System.Windows.Forms.KeyEventHandler(this.keyTextBox_KeyDown);
            this.textBoxRight.MouseEnter += new System.EventHandler(this.keyTextBox_MouseEnter);
            this.textBoxRight.MouseHover += new System.EventHandler(this.textBoxD_MouseHover);
            // 
            // textBoxUp
            // 
            this.textBoxUp.Location = new System.Drawing.Point(296, 127);
            this.textBoxUp.Name = "textBoxUp";
            this.textBoxUp.ReadOnly = true;
            this.textBoxUp.Size = new System.Drawing.Size(130, 20);
            this.textBoxUp.TabIndex = 16;
            this.textBoxUp.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textBoxUp.TextChanged += new System.EventHandler(this.textBoxUp_TextChanged);
            this.textBoxUp.KeyDown += new System.Windows.Forms.KeyEventHandler(this.keyTextBox_KeyDown);
            this.textBoxUp.MouseEnter += new System.EventHandler(this.keyTextBox_MouseEnter);
            this.textBoxUp.MouseHover += new System.EventHandler(this.textBoxD_MouseHover);
            // 
            // buttonDown
            // 
            this.buttonDown.Location = new System.Drawing.Point(63, 186);
            this.buttonDown.Name = "buttonDown";
            this.buttonDown.Size = new System.Drawing.Size(86, 23);
            this.buttonDown.TabIndex = 15;
            this.buttonDown.Text = "Down Arrow";
            this.buttonDown.UseVisualStyleBackColor = true;
            this.buttonDown.Click += new System.EventHandler(this.keyButton_Click);
            // 
            // buttonRight
            // 
            this.buttonRight.Location = new System.Drawing.Point(107, 157);
            this.buttonRight.Name = "buttonRight";
            this.buttonRight.Size = new System.Drawing.Size(86, 23);
            this.buttonRight.TabIndex = 14;
            this.buttonRight.Text = "Right Arrow";
            this.buttonRight.UseVisualStyleBackColor = true;
            this.buttonRight.Click += new System.EventHandler(this.keyButton_Click);
            // 
            // buttonLeft
            // 
            this.buttonLeft.Location = new System.Drawing.Point(15, 157);
            this.buttonLeft.Name = "buttonLeft";
            this.buttonLeft.Size = new System.Drawing.Size(86, 23);
            this.buttonLeft.TabIndex = 13;
            this.buttonLeft.Text = "Left Arrow";
            this.buttonLeft.UseVisualStyleBackColor = true;
            this.buttonLeft.Click += new System.EventHandler(this.keyButton_Click);
            // 
            // buttonUp
            // 
            this.buttonUp.Location = new System.Drawing.Point(63, 128);
            this.buttonUp.Name = "buttonUp";
            this.buttonUp.Size = new System.Drawing.Size(86, 23);
            this.buttonUp.TabIndex = 9;
            this.buttonUp.Text = "Up Arrow";
            this.buttonUp.UseVisualStyleBackColor = true;
            this.buttonUp.Click += new System.EventHandler(this.keyButton_Click);
            // 
            // textBoxS
            // 
            this.textBoxS.ForeColor = System.Drawing.SystemColors.WindowText;
            this.textBoxS.Location = new System.Drawing.Point(296, 78);
            this.textBoxS.Name = "textBoxS";
            this.textBoxS.ReadOnly = true;
            this.textBoxS.Size = new System.Drawing.Size(130, 20);
            this.textBoxS.TabIndex = 8;
            this.textBoxS.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textBoxS.TextChanged += new System.EventHandler(this.textBoxS_TextChanged);
            this.textBoxS.KeyDown += new System.Windows.Forms.KeyEventHandler(this.keyTextBox_KeyDown);
            this.textBoxS.MouseEnter += new System.EventHandler(this.keyTextBox_MouseEnter);
            this.textBoxS.MouseHover += new System.EventHandler(this.textBoxD_MouseHover);
            // 
            // buttonS
            // 
            this.buttonS.Location = new System.Drawing.Point(63, 77);
            this.buttonS.Name = "buttonS";
            this.buttonS.Size = new System.Drawing.Size(86, 23);
            this.buttonS.TabIndex = 7;
            this.buttonS.Text = "S/Cross";
            this.buttonS.UseVisualStyleBackColor = true;
            this.buttonS.Click += new System.EventHandler(this.keyButton_Click);
            // 
            // buttonD
            // 
            this.buttonD.Location = new System.Drawing.Point(107, 48);
            this.buttonD.Name = "buttonD";
            this.buttonD.Size = new System.Drawing.Size(86, 23);
            this.buttonD.TabIndex = 6;
            this.buttonD.Text = "D/Circle";
            this.buttonD.UseVisualStyleBackColor = true;
            this.buttonD.Click += new System.EventHandler(this.keyButton_Click);
            // 
            // buttonA
            // 
            this.buttonA.Location = new System.Drawing.Point(15, 48);
            this.buttonA.Name = "buttonA";
            this.buttonA.Size = new System.Drawing.Size(86, 23);
            this.buttonA.TabIndex = 5;
            this.buttonA.Text = "A/Square";
            this.buttonA.UseVisualStyleBackColor = true;
            this.buttonA.Click += new System.EventHandler(this.keyButton_Click);
            // 
            // textBoxA
            // 
            this.textBoxA.Location = new System.Drawing.Point(221, 49);
            this.textBoxA.Name = "textBoxA";
            this.textBoxA.ReadOnly = true;
            this.textBoxA.Size = new System.Drawing.Size(130, 20);
            this.textBoxA.TabIndex = 4;
            this.textBoxA.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textBoxA.KeyDown += new System.Windows.Forms.KeyEventHandler(this.keyTextBox_KeyDown);
            this.textBoxA.MouseEnter += new System.EventHandler(this.keyTextBox_MouseEnter);
            this.textBoxA.MouseHover += new System.EventHandler(this.textBoxD_MouseHover);
            // 
            // textBoxD
            // 
            this.textBoxD.Location = new System.Drawing.Point(369, 49);
            this.textBoxD.Name = "textBoxD";
            this.textBoxD.ReadOnly = true;
            this.textBoxD.Size = new System.Drawing.Size(130, 20);
            this.textBoxD.TabIndex = 3;
            this.textBoxD.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textBoxD.KeyDown += new System.Windows.Forms.KeyEventHandler(this.keyTextBox_KeyDown);
            this.textBoxD.MouseEnter += new System.EventHandler(this.keyTextBox_MouseEnter);
            this.textBoxD.MouseHover += new System.EventHandler(this.textBoxD_MouseHover);
            // 
            // textBoxW
            // 
            this.textBoxW.Location = new System.Drawing.Point(296, 18);
            this.textBoxW.Name = "textBoxW";
            this.textBoxW.ReadOnly = true;
            this.textBoxW.Size = new System.Drawing.Size(130, 20);
            this.textBoxW.TabIndex = 1;
            this.textBoxW.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textBoxW.KeyDown += new System.Windows.Forms.KeyEventHandler(this.keyTextBox_KeyDown);
            this.textBoxW.MouseEnter += new System.EventHandler(this.keyTextBox_MouseEnter);
            this.textBoxW.MouseHover += new System.EventHandler(this.textBoxD_MouseHover);
            // 
            // buttonW
            // 
            this.buttonW.Location = new System.Drawing.Point(63, 18);
            this.buttonW.Name = "buttonW";
            this.buttonW.Size = new System.Drawing.Size(86, 23);
            this.buttonW.TabIndex = 0;
            this.buttonW.Text = "W/Triangle";
            this.buttonW.UseVisualStyleBackColor = true;
            this.buttonW.Click += new System.EventHandler(this.keyButton_Click);
            // 
            // button2
            // 
            this.button2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.button2.Location = new System.Drawing.Point(307, 532);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 33);
            this.button2.TabIndex = 2;
            this.button2.Text = "OK";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.button3.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.button3.Location = new System.Drawing.Point(388, 532);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 33);
            this.button3.TabIndex = 3;
            this.button3.Text = "Cancel";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button4
            // 
            this.button4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.button4.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.button4.Location = new System.Drawing.Point(469, 532);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(75, 33);
            this.button4.TabIndex = 4;
            this.button4.Text = "Apply";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // toolTipKeys
            // 
            this.toolTipKeys.AutoPopDelay = 5000;
            this.toolTipKeys.InitialDelay = 300;
            this.toolTipKeys.ReshowDelay = 100;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.Black;
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Top;
            this.pictureBox1.Image = global::Properties.Resources.Noter;
            this.pictureBox1.Location = new System.Drawing.Point(0, 0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(551, 108);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(551, 572);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.pictureBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Diva Online KeyBinds";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabControl2.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

		}
	}
}
