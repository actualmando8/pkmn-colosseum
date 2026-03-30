    nofralloc
    mflr r0
    lis r3, 0xcc00
    stw r0, 0x4(r1)
    addi r3, r3, 0x5000
    li r0, -0x29
    stwu r1, -0x2e8(r1)
    stw r31, 0x2e4(r1)
    addi r31, r4, 0x0
    lhz r5, 0xa(r3)
    and r0, r5, r0
    ori r0, r0, 0x80
    sth r0, 0xa(r3)
    addi r3, r1, 0x10
    bl OSClearContext
    addi r3, r1, 0x10
    bl OSSetCurrentContext
    @L_800AEA3C:
    bl fn_800AE7A4
    cmplwi r3, 0x0
    beq @L_800AEA3C
    bl fn_800AE7B4
    lwz r5, lbl_8047A96C(r13)
    lwz r0, 0x8(r5)
    rlwinm. r0, r0, 0, 30, 30
    beq @L_800AEA70
    addis r0, r3, 0x232f
    cmplwi r0, 0x2
    bne @L_800AEA70
    lis r3, 0xdcd1
    addi r3, r3, 0x3
    @L_800AEA70:
    lis r4, 0xdcd1
    addi r0, r4, 0x2
    cmpw r3, r0
    beq @L_800AEAF0
    bge @L_800AEA94
    cmpw r3, r4
    beq @L_800AEAA8
    bge @L_800AEACC
    b @L_800AEDFC
    @L_800AEA94:
    addi r0, r4, 0x4
    cmpw r3, r0
    beq @L_800AEDE4
    bge @L_800AEDFC
    b @L_800AEC58
    @L_800AEAA8:
    li r0, 0x1
    stw r0, 0x0(r5)
    lwz r3, lbl_8047A96C(r13)
    lwz r12, 0x28(r3)
    cmplwi r12, 0x0
    beq @L_800AEDFC
    mtlr r12
    blrl
    b @L_800AEDFC
    @L_800AEACC:
    li r0, 0x1
    stw r0, 0x0(r5)
    lwz r3, lbl_8047A96C(r13)
    lwz r12, 0x2c(r3)
    cmplwi r12, 0x0
    beq @L_800AEDFC
    mtlr r12
    blrl
    b @L_800AEDFC
    @L_800AEAF0:
    lwz r0, lbl_8047A958(r13)
    cmpwi r0, 0x0
    beq @L_800AEB90
    lwz r0, lbl_8047A95C(r13)
    cmplw r5, r0
    bne @L_800AEB48
    lis r3, 0xcdd1
    addi r3, r3, 0x3
    bl fn_800AE7CC
    @L_800AEB14:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEB14
    li r0, 0x0
    lwz r3, lbl_8047A96C(r13)
    stw r0, lbl_8047A95C(r13)
    stw r0, lbl_8047A958(r13)
    lwz r12, 0x2c(r3)
    cmplwi r12, 0x0
    beq @L_800AEDFC
    mtlr r12
    blrl
    b @L_800AEDFC
    @L_800AEB48:
    lis r3, 0xcdd1
    addi r3, r3, 0x1
    bl fn_800AE7CC
    @L_800AEB54:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEB54
    lwz r3, lbl_8047A96C(r13)
    lwz r4, lbl_8047A95C(r13)
    bl fn_800AEE20
    lwz r3, lbl_8047A96C(r13)
    li r4, 0x2
    li r0, 0x0
    stw r4, 0x0(r3)
    lwz r3, lbl_8047A95C(r13)
    stw r0, lbl_8047A958(r13)
    stw r3, lbl_8047A96C(r13)
    stw r0, lbl_8047A95C(r13)
    b @L_800AEDFC
    @L_800AEB90:
    lwz r0, 0x38(r5)
    cmplwi r0, 0x0
    bne @L_800AEC18
    lwz r0, lbl_8047A968(r13)
    cmplw r5, r0
    bne @L_800AEBDC
    lis r3, 0xcdd1
    addi r3, r3, 0x3
    bl fn_800AE7CC
    @L_800AEBB4:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEBB4
    lwz r3, lbl_8047A96C(r13)
    lwz r12, 0x2c(r3)
    cmplwi r12, 0x0
    beq @L_800AEDFC
    mtlr r12
    blrl
    b @L_800AEDFC
    @L_800AEBDC:
    lis r3, 0xcdd1
    addi r3, r3, 0x1
    bl fn_800AE7CC
    @L_800AEBE8:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEBE8
    lwz r3, lbl_8047A96C(r13)
    lwz r4, lbl_8047A968(r13)
    bl fn_800AEE20
    lwz r3, lbl_8047A96C(r13)
    li r0, 0x2
    stw r0, 0x0(r3)
    lwz r0, lbl_8047A968(r13)
    stw r0, lbl_8047A96C(r13)
    b @L_800AEDFC
    @L_800AEC18:
    lis r3, 0xcdd1
    addi r3, r3, 0x1
    bl fn_800AE7CC
    @L_800AEC24:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEC24
    lwz r3, lbl_8047A96C(r13)
    lwz r4, 0x38(r3)
    bl fn_800AEE20
    lwz r3, lbl_8047A96C(r13)
    li r0, 0x2
    stw r0, 0x0(r3)
    lwz r3, lbl_8047A96C(r13)
    lwz r0, 0x38(r3)
    stw r0, lbl_8047A96C(r13)
    b @L_800AEDFC
    @L_800AEC58:
    lwz r0, lbl_8047A958(r13)
    cmpwi r0, 0x0
    beq @L_800AECC0
    lwz r12, 0x30(r5)
    cmplwi r12, 0x0
    beq @L_800AEC7C
    mtlr r12
    addi r3, r5, 0x0
    blrl
    @L_800AEC7C:
    lis r3, 0xcdd1
    addi r3, r3, 0x1
    bl fn_800AE7CC
    @L_800AEC88:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEC88
    li r3, 0x0
    lwz r4, lbl_8047A95C(r13)
    bl fn_800AEE20
    lwz r3, lbl_8047A96C(r13)
    bl fn_800AF1EC
    lwz r3, lbl_8047A95C(r13)
    li r0, 0x0
    stw r0, lbl_8047A958(r13)
    stw r3, lbl_8047A96C(r13)
    stw r0, lbl_8047A95C(r13)
    b @L_800AEDFC
    @L_800AECC0:
    lwz r0, 0x38(r5)
    cmplwi r0, 0x0
    bne @L_800AED7C
    lwz r0, lbl_8047A968(r13)
    cmplw r5, r0
    bne @L_800AED20
    lwz r12, 0x30(r5)
    cmplwi r12, 0x0
    beq @L_800AECF0
    mtlr r12
    addi r3, r5, 0x0
    blrl
    @L_800AECF0:
    lis r3, 0xcdd1
    addi r3, r3, 0x2
    bl fn_800AE7CC
    @L_800AECFC:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AECFC
    lwz r3, lbl_8047A96C(r13)
    li r0, 0x3
    stw r0, 0x0(r3)
    lwz r3, lbl_8047A96C(r13)
    bl fn_800AF1EC
    b @L_800AEDFC
    @L_800AED20:
    lwz r12, 0x30(r5)
    cmplwi r12, 0x0
    beq @L_800AED38
    mtlr r12
    addi r3, r5, 0x0
    blrl
    @L_800AED38:
    lis r3, 0xcdd1
    addi r3, r3, 0x1
    bl fn_800AE7CC
    @L_800AED44:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AED44
    lwz r4, lbl_8047A96C(r13)
    li r0, 0x3
    li r3, 0x0
    stw r0, 0x0(r4)
    lwz r4, lbl_8047A968(r13)
    bl fn_800AEE20
    lwz r0, lbl_8047A968(r13)
    lwz r3, lbl_8047A964(r13)
    stw r0, lbl_8047A96C(r13)
    bl fn_800AF1EC
    b @L_800AEDFC
    @L_800AED7C:
    lwz r12, 0x30(r5)
    cmplwi r12, 0x0
    beq @L_800AED94
    mtlr r12
    addi r3, r5, 0x0
    blrl
    @L_800AED94:
    lis r3, 0xcdd1
    addi r3, r3, 0x1
    bl fn_800AE7CC
    @L_800AEDA0:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEDA0
    lwz r4, lbl_8047A96C(r13)
    li r0, 0x3
    li r3, 0x0
    stw r0, 0x0(r4)
    lwz r4, lbl_8047A96C(r13)
    lwz r4, 0x38(r4)
    bl fn_800AEE20
    lwz r3, lbl_8047A96C(r13)
    lwz r0, 0x38(r3)
    stw r0, lbl_8047A96C(r13)
    lwz r3, lbl_8047A96C(r13)
    lwz r3, 0x3c(r3)
    bl fn_800AF1EC
    b @L_800AEDFC
    @L_800AEDE4:
    lwz r12, 0x34(r5)
    cmplwi r12, 0x0
    beq @L_800AEDFC
    mtlr r12
    addi r3, r5, 0x0
    blrl
    @L_800AEDFC:
    addi r3, r1, 0x10
    bl OSClearContext
    mr r3, r31
    bl OSSetCurrentContext
    lwz r0, 0x2ec(r1)
    lwz r31, 0x2e4(r1)
    addi r1, r1, 0x2e8
    mtlr r0
    blr
