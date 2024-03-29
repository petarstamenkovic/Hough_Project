`ifndef HOUGH_AGENT_SV
    `define HOUGH_AGENT_SV

class hough_agent extends uvm_agent;
    
    //komponente

    hough_driver drv;
    hough_sequencer seqr;
    hough_monitor mon;
    virtual interface hough_interface h_vif;

    //konfiguracija
    hough_config cfg;
    
    `uvm_component_utils_begin(hough_agent)
        `uvm_field_object(cfg,UVM_DEFAULT);
        `uvm_field_object(drv,UVM_DEFAULT);
        `uvm_field_object(seqr,UVM_DEFAULT);
        `uvm_field_object(mon,UVM_DEFAULT);
    `uvm_component_utils_end

    function new(string name = "hough_agent", uvm_component parent = null);
        super.new(name,parent);
    endfunction

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        
        if(!uvm_config_db#(virtual hough_interface)::get(this,"","hough_interface",h_vif))
            `uvm_fatal("NOVIF", {"Virtual interface must be set:",get_full_name(),".h_vif"})

        if(!uvm_config_db#(hough_config)::get(this,"","hough_config",cfg))
            `uvm_fatal("NOCONFIG", {"Config object must be set for:",get_full_name(),".cfg"})

        uvm_config_db#(hough_config)::set(this,"mon","hough_config",cfg);
        uvm_config_db#(hough_config)::set(this,"seqr","hough_config",cfg);
        uvm_config_db#(virtual hough_interface)::set(this,"*","hough_interface",h_vif);

        mon = hough_monitor::type_id::create("mon",this);
        if(cfg.is_active == UVM_ACTIVE) 
        begin
            drv = hough_driver::type_id::create("drv",this);
            seqr = hough_sequencer::type_id::create("seqr",this);
        end

    endfunction : build_phase

    function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        if(cfg.is_active == UVM_ACTIVE) 
        begin
            drv.seq_item_port.connect(seqr.seq_item_export);
        end
    endfunction : connect_phase

endclass : hough_agent 

`endif