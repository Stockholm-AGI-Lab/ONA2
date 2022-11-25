/* 
 * The MIT License
 *
 * Copyright 2020 The OpenNARS authors.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


bool NAR_Bandrobot_Left_executed = false;
double NAR_Bandrobot_amount;
Feedback NAR_Bandrobot_Left(Term args) //* {SELF} val
{
    NAR_Bandrobot_Left_executed = true;
    NAR_Bandrobot_amount = 1.0; //Narsese_NumericAtomValue(args.atoms[2]);
    return (Feedback) {0};
}
bool NAR_Bandrobot_Right_executed = false;
Feedback NAR_Bandrobot_Right(Term args)
{
    NAR_Bandrobot_Right_executed = true;
    NAR_Bandrobot_amount = 1.0; //Narsese_NumericAtomValue(args.atoms[2]);
    return (Feedback) {0};
}
bool NAR_Bandrobot_Pick_executed = false;
Feedback NAR_Bandrobot_Pick()
{
    NAR_Bandrobot_Pick_executed = true;
    return (Feedback) {0};
}
bool NAR_Bandrobot_Drop_executed = false;
Feedback NAR_Bandrobot_Drop()
{
    NAR_Bandrobot_Drop_executed = true;
    return (Feedback) {0};
}

void NAR_Bandrobot(long iterations)
{
    char initial[] = "+++++++++++++++++++++|\n"
                     "---------------------|\n"
                     "                     |\n"
                     "                     |\n"
                     "                     |\n";
    puts(">>NAR Bandrobot start");
    NAR_AddOperation("^left", NAR_Bandrobot_Left);
    NAR_AddOperation("^right", NAR_Bandrobot_Right);
    NAR_AddOperation("^pick", NAR_Bandrobot_Pick); 
    NAR_AddOperation("^drop", NAR_Bandrobot_Drop);
    Shell_ProcessInput("*motorbabbling=0.01");
    long t = 0;
    int minpos = 0.0;
    int maxpos = 20.0;
    int position = 0;
    int targetposition = 3; //maxpos; //maxpos/2;
    bool picked = false, lastpicked = false;
    int successes = 0;
    while(1)
    {
        if(myrand()%10000 > 9000)
        {
            //position = (((double)myrand()/(double)(MY_RAND_MAX)) * maxpos); 
        }
        if(t > 10000)
        {
            Shell_ProcessInput("*motorbabbling=false");
        }
        if(t++ > iterations && iterations != -1)
        {
            break;
        }
        if(NAR_Bandrobot_Left_executed)
        {
            NAR_Bandrobot_Left_executed = false;
            position -= NAR_Bandrobot_amount;
        }
        if(NAR_Bandrobot_Right_executed)
        {
            NAR_Bandrobot_Right_executed = false;
            position += NAR_Bandrobot_amount;
        }
        position = MIN(maxpos, MAX(minpos, position));
        if(picked)
        {
            targetposition = position;
        }
        if(NAR_Bandrobot_Pick_executed)
        {
            NAR_Bandrobot_Pick_executed = false;
            if(position == targetposition)
            {
                picked = true;
            }
        }
        if(NAR_Bandrobot_Drop_executed)
        {
            NAR_Bandrobot_Drop_executed = false;
            picked = false;
        }
        //SLEEP;
        CLEAR_SCREEN;
        char world[sizeof(initial)];
        memcpy(world, initial, sizeof(initial));
        DRAW_LINE(position, 2, 0, 1, (char*) world, 'A');
        DRAW_LINE(targetposition, picked ? 3 : 4, 0, 1, (char*) world, 'o');
        //NAR_AddInputNarsese("<(<(position * targetposition) --> [left]> &/ ?1) =/> aligned>?");
        //NAR_AddInputNarsese("<(<(targetposition * position) --> [left]> &/ ?1) =/> aligned>?");
        NAR_AddInputNarsese("<(?1 * ?2) --> (+ left)>? :\\:");
        puts(world);
        char positionStr[200];
        sprintf(positionStr, "<position --> [left]>. :|: {%f 0.9}", (((float) (position-minpos))/((float) (maxpos-minpos)))/3.0);
        NAR_AddInputNarsese(positionStr);
        char targetpositionStr[200];
        sprintf(targetpositionStr, "<targetposition --> [left]>. :|: {%f 0.9}", (((float) (targetposition-minpos))/((float) (maxpos-minpos)))/3.0);
        NAR_AddInputNarsese(targetpositionStr);
        //if(picked != lastpicked)
        {
            if(picked && !lastpicked)
            {
                NAR_AddInputNarsese("picked. :|:");
            }
            else
            if(!picked && lastpicked)
            {
                NAR_AddInputNarsese("dropped. :|:");
                targetposition = (((double)myrand()/(double)(MY_RAND_MAX)) * (maxpos));
                successes++;
            }
            lastpicked = picked;
        }
        NAR_AddInputNarsese("dropped! :|:");
        printf("ratio=%d time=%ld\n", successes, t);
    }
}
