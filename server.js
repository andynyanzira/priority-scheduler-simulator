const express = require("express");
const { spawn } = require("child_process");
const path = require("path");
const fs = require("fs");

const app = express();
const PORT = process.env.PORT || 3000;

app.use(express.json());
app.use(express.static(path.join(__dirname, "public")));

app.post("/run", (req, res) => {
    const processes = req.body.processes || [];

    const exeName = process.platform === "win32" ? "scheduler.exe" : "scheduler";
    const exePath = path.join(__dirname, exeName);

    if (!fs.existsSync(exePath)) {
        return res.status(500).send(`Simulation executable not found: ${exeName}`);
    }

    const sim = spawn(exePath);

    let output = "";
    let errorOutput = "";

    sim.stdin.write(processes.length + "\n");
    processes.forEach(p => {
        sim.stdin.write(`${p.arrival} ${p.burst} ${p.priority}\n`);
    });
    sim.stdin.end();

    sim.stdout.on("data", data => {
        output += data.toString();
    });

    sim.stderr.on("data", data => {
        errorOutput += data.toString();
    });

    sim.on("close", code => {
        if (code !== 0) {
            res.status(500).send("Simulation error:\n" + errorOutput);
        } else {
            res.send(output);
        }
    });
});

app.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
});