clear all;
close all;

ts = 0.001;

%建立系统传输函数 
sys = tf(400, [1, 50, 0]);
%s域离散化 -> z域
dsys = c2d(sys, ts, 'z');
%取得系统向量
[num den] = tfdata(dsys, 'v');

%变量初始化
u_1 = 0.0;u_2 = 0.0; u_3 = 0.0;
y_1 = 0.0; y_2 = 0; y_3 = 0;

x = [0 0 0]';
error_1 = 0;
error_2 = 0;

for k = 1:1:1000
    time(k) = k * ts;
    
    yd(k) = 1.0;
    kp = 8;
    ki = 0.10;
    kd = 10;
    
    %计算增量
    du(k) = kp*x(1) + kd*x(2) + ki*x(3);
    u(k) = u_1 + du(k);
    
    if u(k) >= 10
        u(k) = 10;
    end
    
    if u(k) <= -10
        u(k) = -10;
    end
    
    y(k) = -den(2) * y_1 - den(3) * y_2 + num(2)*u_1 + num(3) * u_2;
    
    error = yd(k) - y(k);
    u_3 = u_2;
    u_2 = u_1;
    u_1 = u(k);
    y_3 = y_2;
    y_2 = y_1;
    y_1 = y(k);
    x(1) = error - error_1;    

    x(2) = error - 2*error_1 + error_2;
    x(3) = error;
    
    error_2 = error_1;
    error_1 = error;
end

figure(1);
plot(time, yd, 'r', time, y, 'k', 'linewidth',2);
xlabel('time(s)');
ylebel('yd,y');
legend('ideal position value', 'tracking position value');