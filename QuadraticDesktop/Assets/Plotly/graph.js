const EPS = 1e-6;
const divId = "graph";

const config = {
    responsive: true,
    displaylogo: false,
    scrollZoom: true,
    modeBarButtonsToRemove:[
        "select2d",
        "lasso2d",
        "toImage",
        "autoScale2d",
        "resetScale2d"
    ]
};

const emptyLayout = {
    xaxis: {
        tickmode: "linear",
        tick0: 0,
        dtick: 1,
        gridcolor: "#e5e7eb",
        zerolinecolor: "Black"
    },

    yaxis: {
        tickmode: "linear",
        tick0: 0,
        dtick: 1,
        scaleanchor: "x",
        scaleratio: 1,
        gridcolor: "#e5e7eb",
        zerolinecolor: "Black"
    }
};

function createGraphLayout(xMin, xMax) {
    const layout = {
        showlegend: false,
        xaxis: {
            range: [xMin, xMax],
            tickmode: "linear",
            tick0: 0,
            dtick: 1,
            gridcolor: "#e5e7eb",
            zerolinecolor: "Black"
        },

        yaxis: {
            autorange: false,
            tickmode: "linear",
            tick0: 0,
            dtick: 1,
            scaleanchor: "x",
            scaleratio: 1,
            gridcolor: "#e5e7eb",
            zerolinecolor: "Black"
        }
    };

    return layout;
}

function createGraphTrace(a, b, c, xMin, xMax, pointCount) {
    const xValues = [];
    const yValues = [];

    for (let i = 0; i <= pointCount; ++i) {
        const x = xMin + (xMax - xMin) * i / pointCount;

        xValues.push(x);
        yValues.push(a * x * x + b * x + c);
    }

    const graphTrace = {
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

    return graphTrace;
}

function createRootTrace(root, name) {
    const rootTrace = {
        x: [root],
        y: [0],

        type: "scatter",
        mode: "markers",

        marker: {
            color: "red",
            size: 12,
            symbol: "circle",
            line: {
                color: "white",
                width: 2
            }
        },

        hovertemplate:
            "<b>" + name + "</b><br>" +
            "x = %{x:.3f}<br>" +
            "y = %{y:.3f}" +
            "<extra></extra>"
    };

    return rootTrace;
}

function createVertexTrace(a, b, c) {
    const vertexX = -b / (2 * a);
    const vertexY = a * (vertexX * vertexX) + b * vertexX + c;

    const vertexTrace = {
        x: [vertexX],
        y: [vertexY],

        type: "scatter",
        mode: "markers",

        marker: {
            color: "MediumPurple",
            size: 12,
            symbol: "circle",
            line: {
                color: "white",
                width: 2
            }
        },

        hovertemplate:
            "<b>Вершина</b><br>" +
            "x = %{x:.3f}<br>" +
            "y = %{y:.3f}" +
            "<extra></extra>"
    };
    return vertexTrace;
}

function createTraces(a, b, c, x1, x2, nRoots, xMin, xMax, pointCount) {
    const traces = [createGraphTrace(a, b, c, xMin, xMax, pointCount)];

    // При a ≈ 0 функция линейная и вершины параболы нет
    if (Math.abs(a) > EPS) {
        traces.push(createVertexTrace(a, b, c));
    }

    if ((nRoots === 1 || nRoots === 2) && Number.isFinite(x1)) {
        traces.push(createRootTrace(x1, "x₁"));
    }

    if (nRoots === 2 && Number.isFinite(x2)) {
        traces.push(createRootTrace(x2, "x₂"));
    }

    return traces;
}

function drawQuadratic(a, b, c, x1, x2, nRoots) {
    const xMin = -10;
    const xMax = 10;
    const pointCount = 100;

    const traces = createTraces(a, b, c, x1, x2, nRoots, xMin, xMax, pointCount);
    const layout = createGraphLayout(xMin, xMax);

    Plotly.react(divId, traces, layout, config);
}

Plotly.newPlot(divId, [], emptyLayout, config);

window.chrome.webview.addEventListener("message", event => {
    const { a, b, c, x1, x2, nRoots } = event.data;
    drawQuadratic(a, b, c, x1, x2, nRoots);
});