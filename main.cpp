#include "header.h"

bool readMapInfoUntilOK() {
    vector<string> vTmp;
    char line[1024];
    while (fgets(line, sizeof line, stdin)) {
        if (line[0] == 'O' && line[1] == 'K') {
            InitMapInfo(vTmp);
            return true;
        }
        //do something
        vTmp.emplace_back(string(line));
    }
    return false;
}

bool readFrameInfoUntilOK() {
    int rowIndex = 0;
    vector<string> vTmp;
    char line[1024];
    while (fgets(line, sizeof line, stdin)) {
        if (line[0] == 'O' && line[1] == 'K') {
            // 更新数据
            UpDataInfo(vTmp);
            return true;
        }
        // 保存数据
        vTmp.emplace_back(string(line));
        //fprintf(stderr, "%s\n", line);
    }
    return false;
}


int main(int argc, char* argv[]) {
#if SYS_DEBUG
    param1 = atoi(argv[1]) / 100.;
    param2 = atoi(argv[2]) / 100.;
    mapNum = string(argv[3]);
    fprintf(stderr, "Hello World...\npara num :%d ", argc);
    for (int i = 0; i < argc; i++) {
        fprintf(stderr, "%s ", argv[i]);
    }
    //angleKp = param1;
    //angleKd = param2;
    fprintf(stderr, "\n");
    fprintf(stderr, "parm1:%0.2f, parm2:%0.2f\n", param1, param2);
#endif

    //Sleep(15000);

    readMapInfoUntilOK();

    //PrintRobotInfo();
    //PrintBenchInfo();
    puts("OK\n");
    fflush(stdout);

    SysInit();

    //int frame = 0;
    while (scanf("%d", &currentFrameId) != EOF) {

        //fprintf(stderr, "frame id:%d\n", currentFrameId);
        readFrameInfoUntilOK();
        //PrintRobotInfo();
        //PrintBenchInfo();

#if SYS_DEBUG
        {
            // 保存训练收益
            if (currentFrameId == 8999) {
                fprintf(stderr, "money:%d\n", profitMoney);
                fstream fs(string(savePath + mapNum + ".txt"), ios::app | ios::out);
                fs << mapNum << "	" << param1 << "	" << param2 << "	" << profitMoney << endl;
                fs.close();
            }
        }
#endif
        //fprintf(stderr, "-----------------\n");
        // 清空之前的任务 
        priority_queue<TaskStack, vector<TaskStack>, TaskPool> qEmpty;
        qTaskPool.swap(qEmpty);
        for (int i = 0; i < vBenchInfo.size(); i++) {
            TaskStack st;
            CreateMyTaskQueue(i, st, 0, -1);
        }
        //PrintTaskPoolInfo();

        printf("%d\n", currentFrameId);

        Task_Pool();

        IsRobotCollosion();

        printf("OK\n");
        fflush(stdout);
    }

    return 0;
}

