import re
import numpy as np
from matplotlib import pyplot as plt
import sys
import argparse

# clean plots stuff ---------------------
plt.rcParams.update({'font.size': 20})
plt.style.use("ggplot")

small_font = 12
medium_font = 17

plt.rc('font', size=medium_font)
plt.rc('axes', titlesize=medium_font)
plt.rc('axes', labelsize=medium_font)
plt.rc('xtick', labelsize=small_font)
plt.rc('ytick', labelsize=small_font)
plt.rc('legend', fontsize=small_font)
#----------------------------------------

# ==================================== BEGIN FUNCTIONS ====================================

"""
    Plot Photon Burst and Fiber Positions in the x-y plane
"""
def plot2d_func():
    plt.title("{} Photon Stochastic Burst".format(len(xvals)) + " in " + scintillator_type)
    plt.xlabel("x (mm)")
    plt.ylabel("y (mm)")
    xmin = 0
    xmax = 0
    ymin = 0
    ymax = 0
    for i in range(len(xvals)):
        plt.plot(xvals[i]*1000, yvals[i]*1000, "-")
        if (min(xvals[i]) < xmin):
            xmin = min(xvals[i])
        if (max(xvals[i]) > xmax):
            xmax = max(xvals[i])
        if (min(yvals[i]) < ymin):
            ymin = min(yvals[i])
        if (max(yvals[i]) > ymax):
            ymax = max(yvals[i])
    
    # construct and plot fibers based on coordinates
    if (simulate_fibers):
        x_circle = np.array([])
        y_circle = np.array([])
        for i in range(len(x_coordinates)):
            theta = (2*np.pi / 21)*i
            x1 = (diameter/2) * np.cos(theta)
            y1 = (diameter/2) * np.sin(theta)
            x_circle = np.append(x_circle, x1)
            y_circle = np.append(y_circle, y1)
            
        for i in range(len(x_coordinates)):
            plt.plot(x_circle[:]+x_coordinates[i]*1000, y_circle[:]+y_coordinates[i]*1000, '-r')
    
    # plot positions where photon scatters and recalculates velocity vector as well as paths between events
    if (debug):
        print("debug")
        plt.plot(test_x_positions, test_y_positions, 'b-.')
        plt.plot(test_x_positions, test_y_positions, 'go')
        
    plt.xlim([(xmin*1000)-5, (xmax*1000)+5])
    plt.ylim([(ymin*1000)-5, (ymax*1000)+5])
    plt.plot(xvals[0][0]*1000, yvals[0][0]*1000, 'bx')
    print("Emission origin in x,y is: ", str(xvals[0][0]*1000)+"mm", str(yvals[0][0]*1000)+"mm,")
    print("Which is marked by a small blue X.\n")
        
    plt.tight_layout()
    plt.savefig("../out/photonPathsAndFibers.png", bbox_inches="tight") # save as .pdf for higher quality but higher image loading cost
    print("Saved figure as ../out/photonPathsAndFibers.png")
        
#%%
"""
    Plot only Fiber Collisions in the x-y plane
"""
def plot2d_collisions_func():
    plt.style.use("default")
    fig, ax = plt.subplots(2, figsize=(4,5), layout='constrained')
    ax[0].plot(x_end_positions*1000, y_end_positions*1000, "rx",label="Fiber Collisions")
    ax[0].plot(xvals[0][0], yvals[0][0],'g.',label='Origin')
    ax[0].set_xlim(min(x_end_positions*1000), max(x_end_positions*1000))
    ax[0].set_ylim((min(y_end_positions*1000)), max(y_end_positions*1000))
    hist = ax[1].hist2d(x_end_positions, y_end_positions, bins=100, cmap="gist_stern")
    ax[0].set(title="Fiber Collisions in " + scintillator_type)
    ax[1].set(xlabel="x (m)")
    ax[0].set(ylabel="y (m)")
    ax[1].set(ylabel="y (m)")
    for i in range(2):
        ax[i].label_outer()

    # Add colorbar
    cbar = plt.colorbar(hist[3], ax=ax[1])
    cbar.set_label('Counts')

    plt.savefig("../out/fiberCollisions.png", bbox_inches="tight") # save as .pdf for higher quality but higher image loading cost
    print("Saveing image as ../out/fiberCollisions.png")
    plt.style.use("ggplot")

#%%
"""
    Plot Photon Burst in the x-y-z spatial domain
"""

def plot3d_func():
    plt.style.use("default")
    
    fig = plt.figure()
    ax = fig.add_subplot(projection="3d")
    for i in range (len(xvals)):
        ax.plot(xvals[i]*1000, yvals[i]*1000, zvals[i]*1000, '-')
    
    ax.set(title="{} Photon Random Walk".format(len(xvals)) + " in " + scintillator_type, xlabel="x (mm)", ylabel="y (mm)", zlabel="z (mm)")
    
    if (simulate_fibers):
        height = 0
        for i in range (len(zvals)):
            if (max(zvals[i]) > height):
                height = max(zvals[i])
            
        height = height*2
        
        x_fiber = np.array([])
        y_fiber = np.array([])
        z_fiber = np.array([])
        
        for i in range(len(x_coordinates)):
            theta = (2*np.pi / 15)*i
            x1 = (diameter/2) * np.cos(theta)
            y1 = (diameter/2) * np.sin(theta)
            z1 = height/2 if (i%2 == 0) else -height/2
            x_fiber = np.append(x_fiber, x1)
            y_fiber = np.append(y_fiber, y1)
            z_fiber = np.append(z_fiber, z1)
            
            
        for i in range(len(x_coordinates)):
            ax.plot(x_fiber[:]+x_coordinates[i]*1000, y_fiber[:]+y_coordinates[i]*1000, z_fiber[:]*1000, '-r', alpha=0.075)
            
    fig.savefig("../out/full3d.png") # save as .pdf for higher quality but higher image loading cost
    print("Saving figure as ../out/full3d.png")
    

#%%
"""
    Plot Photon Generated Wavelengths
"""
def plot_wavelength_hist_func():
    EMISSION_x_value = np.array([201.0, 203.0, 205.0, 207.0, 209.0, 211.0, 213.0, 215.0, 217.0, 219.0, 221.0, 
                        223.0, 225.0, 227.0, 229.0, 231.0, 233.0, 235.0, 237.0, 239.0, 241.0, 243.0, 
                        245.0, 247.0, 249.0, 251.0, 253.0, 255.0, 257.0, 259.0, 261.0, 263.0, 265.0, 
                        267.0, 269.0, 271.0, 273.0, 275.0, 277.0, 279.0, 281.0, 283.0, 285.0, 287.0, 
                        289.0, 291.0, 293.0, 295.0, 297.0, 299.0, 301.0, 303.0, 305.0, 307.0, 309.0, 
                        311.0, 313.0, 315.0, 317.0, 319.0, 321.0, 323.0, 325.0, 327.0, 329.0, 331.0, 
                        333.0, 335.0, 337.0, 339.0, 341.0, 343.0, 345.0, 347.0, 349.0, 351.0, 353.0, 
                        355.0, 357.0, 359.0, 361.0, 363.0, 365.0, 367.0, 369.0, 371.0, 373.0, 375.0, 
                        377.0, 379.0, 381.0, 383.0, 385.0, 387.0, 389.0, 391.0, 393.0, 395.0, 397.0, 
                        399.0, 401.0, 403.0, 405.0, 407.0, 409.0, 411.0, 413.0, 415.0, 417.0, 419.0, 
                        421.0, 423.0, 425.0, 427.0, 429.0, 431.0, 433.0, 435.0, 437.0, 439.0, 441.0, 
                        443.0, 445.0, 447.0, 449.0, 451.0, 453.0, 455.0, 457.0, 459.0, 461.0, 463.0, 
                        465.0, 467.0, 469.0, 471.0, 473.0, 475.0, 477.0, 479.0, 481.0, 483.0, 485.0, 
                        487.0, 489.0, 491.0, 493.0, 495.0, 497.0, 499.0, 501.0, 503.0, 505.0, 507.0, 
                        509.0, 511.0, 513.0, 515.0, 517.0, 519.0, 521.0, 523.0, 525.0, 527.0, 529.0, 
                        531.0, 533.0, 535.0, 537.0, 539.0, 541.0, 543.0, 545.0, 547.0, 549.0, 551.0, 
                        553.0, 555.0, 557.0, 559.0, 561.0, 563.0, 565.0, 567.0, 569.0, 571.0, 573.0, 
                        575.0, 577.0, 579.0, 581.0, 583.0, 585.0, 587.0, 589.0, 591.0, 593.0, 595.0, 
                        597.0, 599.0])

    EMISSION_y_value = 2*np.array([0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 0.0, 0.0, 0.0, 2.3325e-05, 2.5735e-05, 2.774e-05, 4.345e-05, 
                        6.395e-05, 3.86e-05, 2.735e-05, 2.53e-05, 2.98e-05, 3.375e-05, 3.22e-05, 
                        3.295e-05, 4.1e-05, 3.865e-05, 6.52e-05, 7.65e-05, 0.000134, 0.0002485, 
                        0.000365, 0.0005715, 0.001152, 0.0025435, 0.00458, 0.00749, 0.01147, 
                        0.015545, 0.019625, 0.0226, 0.0229, 0.0212, 0.0191, 0.01755, 0.0171, 
                        0.018, 0.01885, 0.01905, 0.01895, 0.01855, 0.01725, 0.0156, 0.01385, 
                        0.0125, 0.0114, 0.0108, 0.0104, 0.01, 0.0095, 0.00905, 0.00855, 0.00785, 
                        0.00725, 0.0065, 0.00595, 0.00535, 0.00485, 0.0046, 0.0042, 0.0039, 0.00365, 
                        0.0034, 0.0031, 0.00295, 0.0027, 0.00235, 0.0022, 0.002, 0.0018, 0.00165, 
                        0.00145, 0.00135, 0.00125, 0.00115, 0.001, 0.00095, 0.0009, 0.00075, 0.00075, 
                        0.00065, 0.0006, 0.00055, 0.00045, 0.00045, 0.0004, 0.00035, 0.00035, 0.00025, 
                        0.0003, 0.00025, 0.0002, 0.0002, 0.0002, 0.00015, 0.00015, 0.00015, 0.00015, 
                        0.0001, 0.0001, 0.0001, 0.0001, 5e-05, 0.0001, 5e-05, 5e-05, 5e-05, 5e-05, 
                        5e-05, 5e-05, 5e-05, 5e-05, 0.0, 5e-05, 5e-05, 0.0, 5e-05, 0.0, 5e-05, 0.0, 
                        5e-05, 0.0, 0.0, 5e-05, 0.0, 0.0, 5e-05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 0.0, 0.0])

    fig, ax = plt.subplots(2)
    ax[0].hist(wavelengths, bins=100)
    ax[0].set_xlim([min(wavelengths), max(wavelengths)])
    ax[0].set(title="Generated Wavelengths and Emission Probability Spectra", ylabel="Counts")
    
    EMISSION_x_value_2 = np.arange(200, 600, 1)

    interpolated_y = np.interp(EMISSION_x_value_2, EMISSION_x_value, EMISSION_y_value)

    ax[1].plot(EMISSION_x_value_2, interpolated_y)
    ax[1].set_xlim([min(wavelengths), max(wavelengths)])
    ax[1].set(ylabel="Probability", xlabel ="Wavelength (nm)")
    ax[0].label_outer()
    ax[1].label_outer()
    fig.tight_layout()
    
    fig.savefig("../out/generatedWavelengths.png", bbox_inches="tight") # save as .pdf for higher quality but higher image loading cost
    print("Saved figure as ../out/generatedWavelengths.png")

# ==================================== END FUNCTIONS ====================================

# ==================================== BEGIN ARGPARSING/SETUP ====================================
class MyParser(argparse.ArgumentParser):
    def error(self, message):
        sys.stderr.write('error: %s\n' % message)
        self.print_usage()
        sys.exit(2)

parser = MyParser(description="Python plotting script for Monte Carlo photon path results.\
                   Can be called from either the C++ code directly or from the command line by the user.\
                   For interactive plots, use this code in an editor that supports <%matplotlib auto> such as spyder or vscode/jupyter notebook.")

FUNC_MAP = {'plot2d' : plot2d_func,
            'plot2d_collisions' : plot2d_collisions_func,
            'plot3d' : plot3d_func,
            'plot_wavelength_hist' : plot_wavelength_hist_func}

required_function_name = parser.add_argument_group('Required Arguments')
required_function_name.add_argument("-func", choices=FUNC_MAP.keys(), help="Function to be performed", required=True)
scintillator_type = parser.add_argument_group('Scintillator Cocktail Material')
required_function_name.add_argument("-sc_type", help="Scintillator Liquid Body Material [LAB, DIN]", required=True)
parser.add_argument("-fiber_diam", help="Simulated fiber diameter.")
parser.add_argument("-d", help="Plot scattering debugging points (2d only).")

args = parser.parse_args()
diameter = None        # need to initialize
simulate_fibers = True # assumed true to start

if (str(args.func.lower()) not in FUNC_MAP.keys()):
    print("Unsupported function. Check script arguments.")
    exit(1)
function = FUNC_MAP[str(args.func.lower())]

if (str(args.sc_type.lower()) != "lab" and str(args.sc_type.lower()) != "din"):
    print("Unrecognized scintillator type. Check script arguments.")
    exit(1)
scintillator_type = str(args.sc_type.lower())

if (args.fiber_diam != None):
    if (float(args.fiber_diam) < 0):            # TODO - need to also check if fiber radius is larger than spacing
        print("Illegal fiber diameter. Check script arguments.")
        exit(1)
    diameter = float(args.fiber_diam) #mm
else:
    print("No argument passed for fiber diameter; assumed simulation without fibers.")
    simulate_fibers = False

debug = True if (args.d) else False
# ==================================== END ARGPARSING/SETUP ====================================

# ==================================== BEGIN DATA INIT ====================================
"""
    Open fiberPositions.txt and extract x and y coords
"""
if (simulate_fibers):
    with open("../out/fiberPositions.txt", "r") as file:
        # Read all lines
        file_content = file.read()

    # Split the file content by '\n' to separate lines
    lines = file_content.split('\n')

    # Initialize lists to store x and y coordinates
    x_coordinates = []
    y_coordinates = []

    # Iterate through each line
    for line in lines:
        # Split each line by comma
        parts = line.split(',')
        # Extract x and y values and convert to floats
        if (len(parts) > 1):
            x = float(parts[0])
            y = float(parts[1])
            # Append to respective coordinate lists
            x_coordinates.append(x)
            y_coordinates.append(y)

"""
    Open PhotonPaths.txt and extract x,y,z paths
"""
with open(r"../out/PhotonPaths.txt", "r") as file:
    data = file.read()

pattern = r"\[(.*?)\]"  # Regular expression pattern to match text within square brackets

matches = re.findall(pattern, data, re.DOTALL)  # Find all matches of the pattern

xvals = []
yvals = []
zvals = []

for match in matches:
    lines = match.strip().split('\n')
    x_values, y_values, z_values = [], [], []
    for line in lines:
        values = list(map(float, line.split(',')))
        x_values.append(values[0])
        y_values.append(values[1])
        z_values.append(values[2])
        
    xvals.append(np.array(x_values))
    yvals.append(np.array(y_values))
    zvals.append(np.array(z_values))

xvals = np.array(xvals, dtype="object")
yvals = np.array(yvals, dtype="object")
zvals = np.array(zvals, dtype="object")

# --------------------------------------------------

# Define a regular expression pattern to match the wavelength of each entry
pattern = r'; (\d+):'

# Find all matches for the pattern in the data
matches = re.findall(pattern, data)

# Convert matches to integers
wavelengths = [int(match) for match in matches]

# ---------------------------------------------------

# Split the data by lines
lines = data.split('\n')

# Extract only the lines containing "hitFiber=1"
hit_fiber_lines = [line.strip() for line in lines if "hitFiber=1" in line]

x_end_positions = []
y_end_positions = []
z_end_positions = []

for line in hit_fiber_lines:
    start_index = 0
    end_index = line.find("]", start_index)
    
    # Extract the substring containing the array
    array_string = line[start_index:end_index]
    # print (array_string)
    
    values = array_string.split(',')
    x_end_positions.append(float(values[0]))
    y_end_positions.append(float(values[1]))
    z_end_positions.append(float(values[2]))

"""
    Open testPositions.txt and pull the closest fiber testing positions for each photon
"""
if (debug):
    # Define lists to store x and y coordinates
    test_x_positions = []
    test_y_positions = []
    
    with open(r"./testPositions.txt", "r") as file:
        for line in file:
            # Split each line by comma separator
            # print(line)
            if not line.strip():
                continue
            parts = line.strip().split(',')
            # Convert strings to float and append to x and y lists
            test_x_positions.append(float(parts[0])*1000)
            test_y_positions.append(float(parts[1])*1000)

# ==================================== END DATA INIT ====================================

# call specified function
function()