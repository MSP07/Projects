document.addEventListener("DOMContentLoaded", function() {
    const taskForm = document.getElementById("taskForm");
    const taskInput = document.getElementById("taskInput");
    const priorityInput = document.getElementById("priorityInput");
    const deadlineInput = document.getElementById("deadlineInput");
    const subtaskContainer = document.getElementById("subtaskContainer");
    const taskList = document.getElementById("taskList");
    const searchInput = document.getElementById("searchInput");
    const sortPriority = document.getElementById("sortPriority");
    const addSubtaskBtn = document.getElementById("addSubtaskBtn");

    let tasks = [];

    addSubtaskBtn.addEventListener("click", function() {
        const subtaskInput = document.createElement("input");
        subtaskInput.type = "text";
        subtaskInput.placeholder = `Subtask ${subtaskContainer.childElementCount}`;
        subtaskContainer.insertBefore(subtaskInput, addSubtaskBtn);
    });

    taskForm.addEventListener("submit", function(event) {
        event.preventDefault();

        const subtasks = Array.from(subtaskContainer.querySelectorAll("input[type='text']"))
                              .map(input => input.value)
                              .filter(value => value.trim() !== '');

        const taskData = {
            task: taskInput.value,
            priority: priorityInput.value,
            deadline: deadlineInput.value,
            subtasks: subtasks,
            completed: false
        };

        fetch('/add', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(taskData)
        })
        .then(response => response.json())
        .then(task => {
            tasks.push(task);
            renderTasks();
            taskInput.value = '';
            subtaskContainer.querySelectorAll('input[type="text"]').forEach(input => input.remove());
            const newSubtaskInput = document.createElement("input");
            newSubtaskInput.type = "text";
            newSubtaskInput.placeholder = "Subtask 1";
            subtaskContainer.insertBefore(newSubtaskInput, addSubtaskBtn);
            notifyUser(`Task "${task.task}" added successfully!`);
        });
    });

    taskList.addEventListener("click", function(event) {
        if (event.target.classList.contains("completeBtn")) {
            const taskId = event.target.closest("li").dataset.id;
            fetch(`/complete/${taskId}`, {
                method: 'POST'
            })
            .then(response => response.json())
            .then(updatedTask => {
                const task = tasks.find(t => t.id == taskId);
                task.completed = true;
                renderTasks();
                notifyUser("Task marked as completed!");
            });
        }

        if (event.target.classList.contains("deleteBtn")) {
            const taskId = event.target.closest("li").dataset.id;
            fetch(`/delete/${taskId}`, {
                method: 'DELETE'
            })
            .then(response => response.json())
            .then(() => {
                tasks = tasks.filter(t => t.id != taskId);
                renderTasks();
                notifyUser("Task deleted successfully!");
            });
        }
    });

    searchInput.addEventListener("input", function() {
        renderTasks();
    });

    sortPriority.addEventListener("change", function() {
        renderTasks();
    });

    function renderTasks() {
        const query = searchInput.value.toLowerCase();
        let filteredTasks = tasks.filter(task => task.task.toLowerCase().includes(query));

        const sortValue = sortPriority.value;
        if (sortValue !== "All") {
            filteredTasks = filteredTasks.filter(task => task.priority === sortValue);
        }

        taskList.innerHTML = filteredTasks.map(task => `
            <li data-id="${task.id}" class="${task.completed ? 'completed' : ''}">
                <span>${task.task} [Priority: ${task.priority}] [Deadline: ${task.deadline}]</span>
                ${task.completed ? '' : '<button class="completeBtn">Complete</button>'}
                <button class="deleteBtn">Delete</button>
                <ul>${task.subtasks.map(subtask => `<li>${subtask}</li>`).join('')}</ul>
            </li>
        `).join('');
    }

    function notifyUser(message) {
        if (Notification.permission === "granted") {
            new Notification("To-Do List Notification", {
                body: message
            });
        }
    }

    if (Notification.permission !== "granted") {
        Notification.requestPermission();
    }
});
