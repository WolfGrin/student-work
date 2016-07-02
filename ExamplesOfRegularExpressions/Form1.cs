using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Diagnostics;
using System.IO;

namespace ExamplesOfRegularExpressions
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void TestModified(object sender, EventArgs e)
        {
            if (richTextBox1.Modified)
                if (MessageBox.Show("Все несохраненные изменения будут утеряны! Сохранить изменения?", "Внимание!", MessageBoxButtons.YesNo) == DialogResult.Yes)
                    сохранитьToolStripMenuItem_Click(sender, e);
        }

        private void новыйToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TestModified(sender, e);
            richTextBox1.Clear();
            FileName = this.Text = "Новый файл";
        }

        private void открытьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TestModified(sender, e);

            if (openFileDialog1.ShowDialog() == DialogResult.OK && openFileDialog1.FileName.Length > 0)
            {
                try
                {
                    richTextBox1.LoadFile(openFileDialog1.FileName, RichTextBoxStreamType.RichText);
                    FileName = this.Text = openFileDialog1.FileName;
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: Could not read file from disk. Original error: " + ex.Message);
                }
            }

        }
        private string FileName = "Новый файл"; 
        private void сохранитьКакToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (saveFileDialog1.ShowDialog() == DialogResult.OK && saveFileDialog1.FileName.Length > 0)
            {
                try
                {
                    richTextBox1.SaveFile(saveFileDialog1.FileName, RichTextBoxStreamType.RichText);
                    FileName = this.Text = saveFileDialog1.FileName;
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: Could not saved file from disk. Original error: " + ex.Message);
                }
            }
        }

        private void выходToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TestModified(sender, e);
            this.Close();
        }

        private void сохранитьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (FileName != "Новый файл")
            {
                try
                {
                    richTextBox1.SaveFile(FileName, RichTextBoxStreamType.RichText);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: Could not saved file from disk. Original error: " + ex.Message);
                }
            }
            else
            {
                сохранитьКакToolStripMenuItem_Click(sender, e);
            } 
        }

        private void копироватьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            richTextBox1.Copy();
        }

        private void вырезатьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            richTextBox1.Cut();
        }

        private void вставитьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            richTextBox1.Paste();
        }

        private void шрифтToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (fontDialog1.ShowDialog() == DialogResult.OK)
                richTextBox1.SelectionFont = fontDialog1.Font;
        }

        private void цветToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (colorDialog1.ShowDialog() == DialogResult.OK)
                richTextBox1.SelectionColor = colorDialog1.Color;
        }

        private void TestAllText(string pattern)
        {

            Stopwatch sw;
            Regex newReg = new Regex(pattern, RegexOptions.IgnorePatternWhitespace);
            MatchCollection matches;

            sw = Stopwatch.StartNew();                      // начало отрезка времени
            matches = newReg.Matches(richTextBox1.Text);    // поиск 
            sw.Stop();                                      // конец отрезка времени
            label1.Text = string.Format("Time: {0}   Finded: {1}", sw.Elapsed, matches.Count); // выыод затраченого времени 

            // добавление "TEL: " к найденым совпадениям
            richTextBox1.Text = Regex.Replace(richTextBox1.Text, pattern, "TEL: $&", RegexOptions.IgnorePatternWhitespace);
            // подсветка найденых совпадений
            foreach (Match m in newReg.Matches(richTextBox1.Text))
            {
                richTextBox1.Select(m.Index, m.Length);
                richTextBox1.SelectionBackColor = Color.Yellow;
                richTextBox1.DeselectAll();
            }
            richTextBox1.SelectionStart = richTextBox1.TextLength; // перевод курсора в конец строки

        }
        // маска для поиска номеров телефона - Вариант 1, максимально большое количество совпадений (без строгого формата)
        private void test1ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string pattern = @"((?:(?:(?:(?:\+3)\s?[8]\s?[0]\s?)|(?:(?:(?:\+3)\s?[8]\s?))|(?:[8]\s?[0]\s?)|(?:[8]\s?)|(?:[0]\s?))
                    (?:(\(\d{2,5}\))|(?:\d{2,5}))\s?(?:\d{1,3}[-,\s]?)?\d{2,3}[-,\s]?\d{2,3}[-,\s]?)|(?:\(0\d{2,4}\)\s?
                    (?:\d{1,3}[-,\s]?)?\d{2,3}[-,\s]?\d{2,3}[-,\s]?)|(?:\d{1,3}[-]\d{2}[-]\d{2}))";
            TestAllText(pattern);
        }
        // маска для поиска номеров телефона - Вариант 2, с использованием кодов из библиотеки
        private void test2ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string path = @"..\..\PhoneCodeLib.txt";
            if (File.Exists(path))
            {

                //string[] lib1 = File.ReadAllLines(path, Encoding.Default); //открыв файл, считывает все строки в массив строк, закрывает файл. рез-щая строка без переносов
                //или
                string libCodes = File.ReadAllText(path, Encoding.UTF8); //открыв файл, считывает все строки в строку, закрывает файл. рез-щая строка без переносов
                libCodes = libCodes.Replace(", ", ")|(?:0?");
                libCodes = "(?:(?:0?" + libCodes + "))";

                string pattern = @"((?:(?:(?:(?:\+3)\s?[8]\s?[0]\s?)|(?:(?:(?:\+3)\s?[8]\s?))|(?:[8]\s?[0]\s?)|(?:[8]\s?)|(?:[0]\s?))(?:(\(" +
                libCodes + @"\))|(?:" + libCodes + @"))\s?(?:\d{1,3}[-,\s]?)?\d{2,3}[-,\s]?\d{2,3}[-,\s]?)|(?:\(0" + libCodes +
                @"\)\s?(?:\d{1,3}[-,\s]?)?\d{2,3}[-,\s]?\d{2,3}[-,\s]?))";
                TestAllText(pattern);
            }
            else
            {
                MessageBox.Show("Library does not exist!", "Error!!!");
            }
        }
        // маска для поиска номеров телефона - Вариант 3, строгий формат номера телефона +380(код города/оператора)номер телефона
        private void test2ToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            string pattern = @"(\+380(?<kode>\(\d{2,5}\))\d{4,7})"; // при необходимости можно распознать код города через ссылку $kode
            TestAllText(pattern);
        }
    }
}
