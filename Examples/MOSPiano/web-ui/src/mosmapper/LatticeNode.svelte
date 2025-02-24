<script lang='ts'>
    import type { node } from './lattice_math';

    
    //export let s_offset:system = {a:0,b:0}
    export let _node:node
    export let color='gray';
    export let text='';
    export let alt_text:string|undefined = undefined;

    let r = 9;
    let showMenu = false;

    function rightClickContextMenu(e:any){
        console.log('right click')
        showMenu = true;
        //e.preventDefault();
    }

    function handleNodeClick(e:any){
        console.log('node click')
    }
</script>

<!-- svelte-ignore a11y-no-static-element-interactions -->
<!-- svelte-ignore a11y-click-events-have-key-events -->
<circle 
    cx="{_node.p.x}" cy="{_node.p.y}" r="{r}" fill="{color}"
    on:contextmenu|preventDefault={rightClickContextMenu}
    on:click|preventDefault={handleNodeClick}
/>
<text 
    x="{_node.p.x+r+1}" 
    y="{_node.p.y-r}" 
    dominant-baseline="auto" 
    text-anchor="start" 
    fill="{color}"
>{text}</text> 

{#if alt_text}
    <text 
        x="{_node.p.x+r+2}" 
        y="{_node.p.y}" 
        dominant-baseline="hanging" 
        text-anchor="start" 
        fill="{'red'}"
    >{alt_text}</text> 
{/if}

<svelte:window on:click="{() => showMenu = false}" />

{#if showMenu}
<nav>
    <div style="position:fixed;top:{_node.p.y}px;left:{_node.p.x}px;background-color:white;border:1px solid gray;">
        <button on:click="{() => console.log('add item')}">Add item</button>
        <button on:click="{() => console.log('print')}">Print</button>
        <button on:click="{() => console.log('zoom')}">Zoom</button>
    </div>
</nav>
{/if}