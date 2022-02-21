import dash
import dash_leaflet as dl
from dash import dcc
from dash import html
from dash import dash_table
from dash.dependencies import Input, Output
import plotly.express as px
import pandas as pd
import base64

from aac import Aac as AnimalShelter

###########################
# Data Manipulation / Model
###########################

username = "aacuser"
password = "password"
shelter = AnimalShelter(username, password)


# class read method must support return of cursor object and accept projection json input
df = pd.DataFrame.from_records(shelter.read({}, 1))
data = df.to_dict('records')
cols = [{"name": i, "id": i, "deletable": False, "selectable": True}
		for i in df.columns
		if i not in ["_id", "monthyear", "datetime", "location_lat", "location_long", "age_upon_outcome_in_weeks"]
	]
searchDropdownOptions = [col for col in df.columns if col not in [""]]
searchDropdownOptions.insert(0,'Search Field')
sortDropdownOptions = ['Sort Field', 'datetime', 'age_upon_outcome_in_weeks']

image_filename = 'logo.png'
encoded_image = base64.b64encode(open(image_filename, 'rb').read())

def get_updated_data(field, value, is_searching, training):
  if is_searching:
    animals_cursor = shelter.read({ field : value })
  elif training != 'Reset':
    if training == 'Water Rescue':
      animals_cursor = shelter.read({"animal_type":"Dog","breed":{"$in":["Labrador Retriever Mix", "Chesapeake Bay Retriever", "Newfoundland"]}, "sex_upon_outcome":"Intact Female","age_upon_outcome_in_weeks":{"$gte":26},"age_upon_outcome_in_weeks":{"$lte":156}})
    if training == 'Mountain Rescue':
      animals_cursor = shelter.read({"animal_type":"Dog","breed":{"$in":["German Shepherd","Alaskan Malamute","Old English Sheepdog","Siberian Husky","Rottweiler"]}, "sex_upon_outcome":"Intact Male","age_upon_outcome_in_weeks":{"$gte":26},"age_upon_outcome_in_weeks":{"$lte":156}})
    if training == 'Disaster Rescue':
      animals_cursor = shelter.read({"animal_type":"Dog","breed":{"$in":["Doberman Pinscher", "German Shepherd", "Golden Retriever", "Bloodhound", "Rottweiler"]}, "sex_upon_outcome":"Intact Male","age_upon_outcome_in_weeks":{"$gte":26},"age_upon_outcome_in_weeks":{"$lte":300}})
  else:
    animals_cursor = shelter.read({})
    
  df = pd.DataFrame.from_records(animals_cursor)
  return df.to_dict('records')

#########################
# Dashboard Layout / View
#########################
app = dash.Dash()
app.layout = html.Div([
	html.Div(
  style={'width':'100%', 'display':'flex', 'justify-content':'space-around', 'align-items':'space-around'},
  children=[
    html.Div(id='hidden-div', style={'display':'none'}),
    html.Center(html.B(html.H1('Garzioso Salvare\'s Dog Training Candidates Dashboard'))),
    html.Img(
      src="data:image/png;base64,{}".format(encoded_image.decode()),
      style={'width':'100px', 'height':'100px'}
      ),
  ]),
  html.Center(html.B(html.H3('Logan Parker'))),
  html.Hr(),
  html.Div([
    html.H3('Search:'),
		dcc.Dropdown(
			id='search-field-id',
			options=searchDropdownOptions,
			value=searchDropdownOptions[0],
		),
		dcc.Input(
			id='search-text-id',
			value='Search Value',
			type='text'
		),
  ]),
  #   html.H3('Sort:'),
  #     dcc.Dropdown(
	# 		id='sort-field-id',
	# 		options=sortDropdownOptions,
	# 		value=sortDropdownOptions[0],
	# 	),
  #   dcc.Checklist(
  #     id='sort-direction',
  #     options=['^']
  #     )
	#  ]),
	# html.Div(),
	dcc.RadioItems(
		id="training-types-id",
		options=[
				{"label": "Reset", "value": "Reset"},
				{"label": "Water Rescue", "value": "Water Rescue"},
				{"label": "Mountain Rescue", "value": "Mountain Rescue"},
				{"label": "Disaster Rescue", "value": "Disaster Rescue"},
		],
		value="Reset"
	),
	dash_table.DataTable(
		id='datatable-id',
		columns=cols,
		data=data,
		selected_columns=[],
		selected_rows=[],
		page_action="native",
		page_current=0,
		page_size=10,
		derived_viewport_data=data,
),
	html.Br(),
	html.Hr(),
 	html.Div(
		style={'width':'100%', 'display':'flex', 'justify-content':'center'},
		children = [
		dcc.Graph(id="pie-chart"),
		html.Div(
			id="map-id",
			className=f"col s{30.0} m{-97.40}",
		)
	])
])

#############################################
# Interaction Between Components / Controller
#############################################
#This callback will highlight a row on the data table when the user selects it
@app.callback(
	Output('datatable-id', 'style_data_conditional'),
	[Input('datatable-id', 'selected_columns')]
)
def update_styles(selected_columns):
  return [{ 'if': { 'column_id': i }, 'background_color': '#D2F3FF'} for i in selected_columns]


@app.callback(
	Output('datatable-id', 'data'),
	[Input('search-field-id', 'value'),
  Input('search-text-id', 'value'),
  Input('training-types-id', 'value')]
)
def update_query(selected_column, input_text, training):
  is_searching = selected_column != 'Search Field' and training != 'Reset'
  return get_updated_data(selected_column, input_text, is_searching, training)

@app.callback(
	Output('map-id', 'children'),
	[Input('datatable-id', 'data')])
def update_map(viewData):
  markers = []
  for animal in viewData:  
    markers.append(dl.TileLayer(id="base-layer-id"))
    markers.append(
        dl.Marker(position=[animal["location_lat"],animal["location_long"]], children=[
        dl.Tooltip(animal["breed"]),
        dl.Popup([
          html.H1("Animal Name"),
          html.P(animal["name"])
        ]) 
      ])
    )
	# Austin TX is at [30.75,-97.48]   
  return [
		dl.Map(style={'width': '1000px', 'height': '500px'}, center=[30.75,-97.48], zoom=10, children=markers)
	]
  
@app.callback(
	Output("pie-chart", "figure"),
	[Input('datatable-id', 'data')])
def update_chart(data):
  breeds = {}
  for animal in data:
    breed = animal['breed']
    if breed in breeds.keys():
      breeds[breed] = breeds[breed] + 1
    else:
      breeds[breed] = 1
	
  return px.pie(df, values=[breeds[key] for key in breeds.keys()], names=breeds.keys())
    
if __name__ == "__main__":
	app.run_server(host="localhost", debug=True)
 