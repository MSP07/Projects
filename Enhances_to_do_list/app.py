from flask import Flask, render_template, request, redirect, url_for, jsonify
import json
import os
from datetime import datetime

app = Flask(__name__)

TASKS_FILE = "tasks.json"

def load_tasks():
    if os.path.exists(TASKS_FILE):
        with open(TASKS_FILE, "r") as file:
            return json.load(file)
    return []

def save_tasks(tasks):
    with open(TASKS_FILE, "w") as file:
        json.dump(tasks, file)

@app.route('/')
def index():
    tasks = load_tasks()
    return render_template('index.html', tasks=tasks)

@app.route('/add', methods=['POST'])
def add_task():
    tasks = load_tasks()
    task_data = request.json
    task = {
        "id": len(tasks) + 1,
        "task": task_data['task'],
        "priority": task_data['priority'],
        "deadline": task_data['deadline'],
        "completed": False,
        "subtasks": task_data['subtasks']
    }
    tasks.append(task)
    save_tasks(tasks)
    return jsonify(task)

@app.route('/complete/<int:task_id>', methods=['POST'])
def complete_task(task_id):
    tasks = load_tasks()
    tasks[task_id]["completed"] = True
    save_tasks(tasks)
    return jsonify({"status": "success"})

@app.route('/delete/<int:task_id>', methods=['DELETE'])
def delete_task(task_id):
    tasks = load_tasks()
    tasks = [task for task in tasks if task["id"] != task_id]
    save_tasks(tasks)
    return jsonify({"status": "success"})

@app.route('/search', methods=['GET'])
def search_tasks():
    query = request.args.get('query', '').lower()
    tasks = load_tasks()
    filtered_tasks = [task for task in tasks if query in task['task'].lower()]
    return jsonify(filtered_tasks)

if __name__ == '__main__':
    app.run(debug=True)
