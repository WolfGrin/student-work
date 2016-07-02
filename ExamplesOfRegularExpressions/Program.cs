/*
    Тектовый редактор, с минимальным набором функций и возможностью поиска номеров телефонов, используя регулярные выражения
    Для тестирования предоставленны 3 способа поиска телефонов:
    1 - поиск наибольшего колличества телефонов (работает дольше, есть незначительня вероятность ошибочного выявления)
    2 - поиск с использованием базы кодов (работает быстрее первого варианта, ошибочных выявлений намного меньше, более строгий формат)
    3 - наиболее жесткий заданый формат для поиска, наиболее быстрый поиск, ошибочных срабатываний нет, возможность определить указаный код города/оператора по ссылке
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace ExamplesOfRegularExpressions
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
