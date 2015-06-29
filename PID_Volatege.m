close all;
clear all;

ts = 0.001;

%电池源电压
Vs = 4.7;
%电池输出电流，Is < 1A
Is = 0; 

%支路上两电池的等效电阻
R1 = 0;
R2 = 0;

%优化器实际输入电压
Vin2 = 20;
Vin1 = 20;

%PWM 占空比
D = 0.1;

%负载电阻
RL = 9.4;

%误差变量
error = 0.0;
error_1 = 0.0;
error_2 = 0.0;


for k = 1:1:10000
    time(k) = k * ts;
  
    
    %目标输出
    yd(k) = 9.4;
    
    %PID 参数
    kp = 0.2;
    ki = 0.001;
    kd = 0.001;
    
    %if rem(k, 500) == 0
    %  Vin1 = rand(1) + Vin1;
    %end
    
    %PID调节过程
    du(k) = kp * (error - error_1) + ki*(error) + kd*(error - 2*error_1 + error_2);
    D = D + 0.05*error;    
    if D > 0.9
        D = 0.9;
    end
    
    if D < 0.1
        D = 0.1;
    end
        d(k) = D;

    y1(k) = Vin1 * D / (1 - D);
    y2(k) = Vin2 * D / (1 - D);
    
    y(k) =  y1(k) + y2(k);
    
    vin1(k) = Vin1;
    vin2(k) = Vin2;
    I1(k) = y1(k) / Vin1;
    I2(k) = y2(k) / Vin2; 

    Vin1 = Vs - (y1(k) / Vin1) * R1;
    Vin2 =  Vs - (y2(k) / Vin2) * R2;
   
    error = yd(k) - y(k);
    error_2 = error_1;
    error_1 = error;
end

figure(1);
hold;
plot(y,'r','linewidth',2);
plot(y1,'b','linewidth',2);
plot(y2,'g','linewidth',2);
legend('y','y1','y2');

figure(2);
hold;
plot(vin1,'linewidth',2);
plot(vin2,'linewidth',2);

legend('Vin1','Vin2');

figure(3);
plot(d,'linewidth',2);
legend('d');