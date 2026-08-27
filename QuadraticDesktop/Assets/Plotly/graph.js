"use strict";

const config = {
    responsive: true,
    displaylogo: false,
    scrollZoom: true
};

const emptyLayout = {
    margin: {
        left: 0,
        right: 0,
        top: 0,
        bottom: 0
    },

    showlegend: false,

    xaxis: {
        title: "x",
        tickmode: "linear",
        tick0: 0,
        dtick: 1,
        gridcolor: "#e5e7eb",
        zerolinecolor: "#111827"
    },

    yaxis: {
        title: "y",
        tickmode: "linear",
        tick0: 0,
        dtick: 1,
        scaleanchor: "x",
        scaleratio: 1,
        gridcolor: "#e5e7eb",
        zerolinecolor: "#111827"
    }
};

function drawQuadratic(a, b, c) {
    const xValues = [];
    const yValues = [];

    const xMin = -10;
    const xMax = 10;
    const pointCount = 100;

    for (let i = 0; i <= pointCount; ++i) {
        const x = xMin + (xMax - xMin) * i / pointCount;
        const y = a * x * x + b * x + c;

        xValues.push(x);
        yValues.push(y);
    }

    const trace = {
        x: xValues,
        y: yValues,
        type: "scatter",
        mode: "lines",

        line: {
            color: "#ef4444",
            width: 3
        },

        hovertemplate:
            "x = %{x:.3f}<br>" +
            "y = %{y:.3f}" +
            "<extra></extra>"
    };

    const layout = {
        margin: {
            left: 0,
            right: 0,
            top: 0,
            bottom: 0
        },

        showlegend: false,

        xaxis: {
            title: "x",
            range: [xMin, xMax],
            tickmode: "linear",
            tick0: 0,
            dtick: 1,
            gridcolor: "#e5e7eb",
            zerolinecolor: "#111827"
        },

        yaxis: {
            title: "y",
            autorange: false,
            tickmode: "linear",
            tick0: 0,
            dtick: 1,
            scaleanchor: "x",
            scaleratio: 1,
            gridcolor: "#e5e7eb",
            zerolinecolor: "#111827"
        }
    };

    Plotly.react("graph", [trace], layout, config);
}

Plotly.newPlot("graph", [], emptyLayout, config);

window.chrome.webview.addEventListener("message", event => {
    const { a, b, c } = event.data;

    drawQuadratic(a, b, c);
});