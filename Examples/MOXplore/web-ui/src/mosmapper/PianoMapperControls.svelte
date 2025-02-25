<script lang='ts'>
    //import { Grid, Space,  Button } from '@svelteuidev/core';
    //import Slider from '$lib/components/Slider.svelte';
    import Knob from '../lib/Knob.svelte';

    export let Down:()=>void;
    export let Up:()=>void;
    export let base_tune:number;
    export let octave_exp:number;
    export let G:number;
    export let G_s:number;
    export let window_width:number;
    export let window_offset:number;
    //export let x_scale_factor:number;
    export let freeze_mapping:boolean;
    export let cpa_slider_min:number;
    export let cpa_slider_max:number;

    let base_tune_knob: any;
    let octave_exp_knob: any;
    let G_knob: any;
    let G_s_knob: any;
    let window_width_knob: any;
    let window_offset_knob: any;

    let depth_knob: any;
    let depth: number = 3;
    let last_depth: number = 3;
    function update_depth(dpth:number){
        while(last_depth > dpth){
            Up();
            last_depth--;
        }
        while(last_depth < dpth){
            Down();
            last_depth++;
        }
    }
    $: update_depth(Math.round(depth));


    // Handle parameter value changes from the plugin
    globalThis.SPVFD = (paramIdx: number, val: number) => {
        switch (paramIdx) {
            case 0:
                base_tune_knob?.setValueFromPlugin(val); 
                break;
            case 1:
                octave_exp_knob?.setValueFromPlugin(val);
                break;
            case 2:
                G_knob?.setValueFromPlugin(val);
                break;
            case 3:
                G_s_knob?.setValueFromPlugin(val);
                break;
            case 4:
                window_width_knob?.setValueFromPlugin(val);
                break;
            case 5:
                window_offset_knob?.setValueFromPlugin(val);
                break;
            case 6:
                depth_knob?.setValueFromPlugin(val);
                break;
        }
    };

</script>

<style>
    .outercontainer {
        position: absolute;
        top: 0;
        left: 0;
        right: 320px;
        height:220px;
        background-color: #FFB319;
    }    
    .container-1 {
        position: absolute;
        top:0;
        left: 0;
        width:650px;
        display: inline-grid;
        justify-items: center;
        grid-template-columns: repeat(7, 1fr);
    }
    .container-2 {
        position: absolute;
        top:110px;
        left: 0;
        width:650px;
        display: inline-grid;
        justify-items: center;
        grid-template-columns: repeat(7, 1fr);
    }
    .item{
        grid-column: span 1;
        align-self: center;
    }
    .item-1 {grid-column: 1 / span 1;}
    .item-2 {grid-column: 2;}
    .item-3 {grid-column: 3;}
    .item-4 {grid-column: 4;}
    .item-5 {grid-column: 5;}
    .item-6 {grid-column: 6;}
    .item-7 {grid-column: 7;}
</style>

<div class="outercontainer">
    <div class="container-1">
        <div class="item item-1">
            <Knob
                bind:this={base_tune_knob}
                bind:value={base_tune}
                minValue={-1}
                maxValue={1}
                label="base"
                paramId={0}
                precision={3}
                fineness={3}
            />
        </div> 
        <div class="item item-2">
            <Knob
                bind:this={octave_exp_knob}
                bind:value={octave_exp}
                minValue={0.1}
                maxValue={2.2}
                label="stretch"
                paramId={1}
                precision={3}
                fineness={10}
            />
        </div>
        <div class="item item-3">
            <Knob
                bind:this={G_knob}
                bind:value={G}
                minValue={0.5}
                maxValue={1}
                label="angle coarse"
                paramId={2}
                precision={3}
                fineness={10}
            />
        </div>
        <div class="item item-4">
            <Knob
                bind:this={G_s_knob}
                bind:value={G_s}
                bind:minValue={cpa_slider_min}
                bind:maxValue={cpa_slider_max}
                label="angle fine"
                paramId={3}
                precision={3}
                fineness={10}
            />
        </div>
        <div class="item item-5">
            <Knob
                bind:this={window_width_knob}
                bind:value={window_width}
                minValue={2}
                maxValue={36}
                label="period"
                paramId={4}
                precision={0}
                fineness={3}
            />
        </div>
        <div class="item item-6">
            <Knob
                bind:this={window_offset_knob}
                bind:value={window_offset}
                minValue={1}
                maxValue={window_width}
                label="offset"
                paramId={5}
                precision={0}
                fineness={2}
            />


            <!--start midi {start_midi}
            <Slider bind:value={start_midi} min={30} max={90} step={1}/>
            <Space h={15}/>
            <Checkbox bind:checked={freeze_mapping} label="freeze mapping" />-->
        </div>

        <div class="item item-7">
            <Knob
                bind:this={depth_knob}
                bind:value={depth}
                minValue={0}
                maxValue={8}
                label="depth"
                paramId={6}
                precision={0}
            />
        </div>

    </div>
    <div class="container-2">
        <!--
        <div style="margin:auto;width:50%">
            <button 
                on:click={Down}
                disabled={freeze_mapping}
            >+</button>
        </div>
        
        <div style="margin:auto;width:50%">
            <button
                on:click={Up}
                disabled={freeze_mapping}
            >-</button>
        </div>
        -->
    </div>
</div>