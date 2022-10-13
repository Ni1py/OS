using System;
using System.IO;
using System.Collections.Generic;

namespace MileMurConversion
{
    class Program
    {
        const string mealyToMoore = "mealy-to-moore";
        const string mooreToMealy = "moore-to-mealy";
        const char condition = 'S';

        static void Main(string[] args)
        {
            StreamReader reader = new StreamReader(args[1]);
            StreamWriter writer = new StreamWriter(args[2]);
            if (args[0] == mealyToMoore)
            {
                WriteMatrix(writer, MealyToMoore(ReadMatrix(reader)));
            } else if (args[0] == mooreToMealy)
            {
                WriteMatrix(writer, MooreToMealy(ReadMatrix(reader)));
            }
        }

       private static List<List<string>> ReadMatrix(StreamReader reader)
       {
           List<List<string>> matrix = new List<List<string>>();
           string? line;
           while ((line = reader.ReadLine()) != null)
           {
               List<string> lineList = new List<string>();
               string[] parseLine = line.Split(';');
               foreach (string element in parseLine)
               {
                   lineList.Add(element);
               }
               matrix.Add(lineList);
           }

           return matrix;
       }

        private static void WriteMatrix(StreamWriter writer, List<List<string>> matrix)
        {
            for (var j = 0; j < matrix.Count; j++)
            {
                for (var i = 0; i < matrix[j].Count; i++)
                {
                    if (i == matrix[j].Count - 1)
                    {
                        writer.WriteLine(matrix[j][i].Split(';')[0]);
                        Console.WriteLine(matrix[j][i].Split(';')[0]);
                    }
                    else
                    {
                        writer.Write(matrix[j][i]);
                        Console.Write(matrix[j][i]);
                    }
                }
            }
        }

        private static List<List<string>> MealyToMoore(List<List<string>> miliTable)
       {
           Dictionary<string, string> newStates = new Dictionary<string, string>();
           List<string> inputChar = new List<string>();
           int k = 0;

           for (var j = 0; j < miliTable.Count; j++)
           {
               if (j > 0)
               {
                   for (var i = 0; i < miliTable[j].Count; i++)
                   {
                       if (i == 0)
                       {
                           inputChar.Add(miliTable[j][i]);
                       }
                       if (i > 0)
                       {
                           if (!newStates.ContainsKey(miliTable[j][i]))
                           {
                               newStates.Add(miliTable[j][i], condition + k.ToString());
                               k++;
                           }
                       }
                   }
               }
           }
           var rowCount = 2 + inputChar.Count;
           List<List<string>> mooreTable = new List<List<string>>();
           for (var l = 0; l < rowCount; l++)
            {
                mooreTable.Add(new List<string>());
            }
           var indexInputChar = 0;

           for (var l = 0; l < rowCount; l++)
           {
                if (l == 0)
                {
                    mooreTable[l].Add(";");
                    foreach (var element in newStates)
                    {
                        mooreTable[l].Add(element.Key.Split('/')[1] + ';');
                    }
                } else if (l == 1)
                {
                    mooreTable[l].Add(";");
                    foreach (var element in newStates)
                    {
                        mooreTable[l].Add(element.Value + ';');
                    }
                } else if (l > 1)
                {
                    mooreTable[l].Add(inputChar[indexInputChar] + ';');
                    indexInputChar++;
                    foreach (var element in newStates)
                    {
                        var index = -1;
                        for (var j = 0; j < miliTable.Count; j++)
                        {
                            for (var i = 0; i < miliTable[j].Count; i++)
                            {
                                if (i == index && j == indexInputChar)
                                {
                                    mooreTable[l].Add(newStates[miliTable[j][i]] + ';');
                                    
                                }
                                if (miliTable[j][i] == element.Key.Split('/')[0])
                                {
                                    index = i;
                                }
                            }
                        }
                    }
                }
           }

           return mooreTable;
        }

        private static List<List<string>> MooreToMealy(List<List<string>> matrix)
        {
            List<List<string>> mealyTable = new List<List<string>>();
            List<string> outputs = matrix[0];
            List<string> states = matrix[1];
            List<string> _states = new List<string>();
            foreach (var element in states)
            {
                _states.Add(element + ';');
            }
            mealyTable.Add(_states);
            for (var j = 2; j < matrix.Count; j++)
            {
                List<string> row = new List<string>();
                for (var i = 0; i < matrix[j].Count; i++)
                {
                    if (i == 0)
                    {
                        row.Add(matrix[j][i] + ';');
                    }
                    else
                    {
                        row.Add(matrix[j][i].Split(';')[0] + '/' + outputs[states.IndexOf(matrix[j][i].Split(';')[0])] + ';');
                    }
                }
                mealyTable.Add(row);
            }

            return mealyTable;
        }
    }
}
