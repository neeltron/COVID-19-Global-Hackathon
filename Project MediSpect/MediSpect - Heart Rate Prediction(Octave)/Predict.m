data = load('dataset.txt') % Load the dataset
X = data(:,1) % Setting the value of X to the first column of dataset
y = data(:,2) % Setting the value of y to the second column of the dataset
m = length(y);
plot(X, y, 'bo', 'MarkerSize', 10) % Plotting the data
X = [ones(m,1), data(:,1)]
theta = zeros(2,1); % Initializing theta
J = computeCost(X, y, theta); % Setting J equal to the cost function
iterations = 90000 % number of iterations
alpha = 0.0004; % Learning Rate
theta = gradientDescent(X,y,theta,alpha,iterations) % Adjusting theta to the point where J(Cost Function) is minimum
hold on;
plot(X(:,2), X*theta, 'r-', 'MarkerSize', 20) % Plotting a line with slope theta
n = input("Enter the ID to predict Heart Rate: ") % Taking input to predict population
predicted_heartrate = [1, n]*theta