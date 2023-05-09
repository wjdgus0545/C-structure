#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define MAX_PROCESS 1000
#define MAX_TIME_QUANTUM 10000

// PCB는 ID 순서대로 생성된다고 가정.

typedef struct {
    int pid;            // 프로세스 ID
    int generate_time;   // 생성 시간
    int burst_time;     // 실행 시간
    int remaining_time; // 남은 실행 시간
    int completed;      // 프로세스 정상적으로 수행 완료되었는지
} PCB;

int n;                          // 프로세스 수
PCB processes[MAX_PROCESS];     // 프로세스 배열
int time_quantum;                // 시간 할당량

// 프로세스 정보 입력 함수
void input_processes() {
    printf("\n 프로세스 수: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("\n 프로세스 %d\n - 생성 시간: ", i);
        scanf("%d", &processes[i].generate_time);
        printf(" - 실행 시간: ", i);
        scanf("%d", &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].pid = i;
        processes[i].completed = 0;
    }
    printf("\n----------------------------------------------------------------------------------------\n");
    printf("\n 시간 할당량(time quantum): ");
    scanf("%d", &time_quantum);
    printf("\n");
}

void print_info_complete(int current_tm, int cur_proc) {
    printf(" ..........현재 시간 : %3d,   프로세스 %d 수행 완료\n\n", current_tm, cur_proc);
}

void print_info(int current_tm, int pre_proc, int cur_proc, bool isCompleted) {

    int rt = processes[cur_proc].remaining_time;
    int rem_tm = isCompleted ? rt + time_quantum : rt;

    printf("\n");
    if (pre_proc < 0)
        printf(" 현재 시간: %3d,\t이전 프로세스 ID: -,\t  시작 프로세스 ID: %d", current_tm, cur_proc);
    else
        printf(" 현재 시간: %3d,\t이전 프로세스 ID: %d,\t  시작 프로세스 ID: %d", current_tm, pre_proc, cur_proc);

    printf(" (남은 실행시간: %d)\n", rem_tm);
    
}


// 라운드 로빈 스케줄링 함수
void round_robin() {
    int current_time = processes[0].generate_time;      // 현재 시간 (첫 프로세스의 생성 시간 값으로 초기화)
    int completed_processes = 0;                        // 완료된 프로세스 수
    int current_process = 0;                            // 현재 실행 중인 프로세스 인덱스
    int pre_process = -1;                               // 이전 프로세스 인덱스
    int flag = 0;

    while (completed_processes < n) {

        flag = 0;

        // 현재 실행 중인 프로세스의 남은 실행 시간이 시간 할당량보다 큰 경우 처리
        if (processes[current_process].remaining_time > time_quantum) {
            print_info(current_time, pre_process, current_process, false);
            Sleep(time_quantum * 400);
            processes[current_process].remaining_time -= time_quantum;
            current_time += time_quantum;
            pre_process = current_process;
            current_process = (current_process + 1) % n;
            continue;
        }

        // 현재 실행 중인 프로세스의 남은 실행 시간을 시간 할당량만큼 감소시킴
        if (processes[current_process].remaining_time > 0) {
            processes[current_process].remaining_time -= time_quantum;
        }

        // 남은 실행 시간이 0보다 작거나 같으면 프로세스 완료 처리
        if (!processes[current_process].completed && processes[current_process].remaining_time <= 0) {
            completed_processes++;
            processes[current_process].completed = 1;
            print_info(current_time, pre_process, current_process, true);
            Sleep((processes[current_process].remaining_time + time_quantum) * 400);
            print_info_complete(current_time + processes[current_process].burst_time, current_process);
            current_time += time_quantum;
            flag = 1;
        }
        
        // 현재 프로세스를 이전 프로세스로.
        if (flag) pre_process = current_process;
        // 다음 프로세스 선택
        current_process = (current_process + 1) % n;
    }
}

int main() {
    input_processes();
    round_robin();

    return 0;
}
