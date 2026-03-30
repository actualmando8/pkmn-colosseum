    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    lwz r3, 0x8(r3)
    bl fn_8019FF48
    lbz r0, 0x46(r31)
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    cmplwi r0, 0x0
    li r9, 0x0
    bne @L_8013CE98
    li r3, 0x1
    b @L_8013D094
    @L_8013CE98:
    cmplwi r3, 0x0
    bne @L_8013CEA8
    li r3, 0x0
    b @L_8013D094
    @L_8013CEA8:
    beq @L_8013CEB4
    lwz r4, 0xc(r3)
    b @L_8013CEB8
    @L_8013CEB4:
    li r4, 0x0
    @L_8013CEB8:
    cmplwi r4, 0x0
    bne @L_8013CEC8
    li r3, 0x0
    b @L_8013D094
    @L_8013CEC8:
    lwz r5, 0x8(r4)
    cmplwi r5, 0x0
    mr r3, r5
    bne @L_8013CEE0
    li r3, 0x0
    b @L_8013D094
    @L_8013CEE0:
    li r10, 0x1
    b @L_8013D020
    @L_8013CEE8:
    cmpwi r0, 0xb
    beq @L_8013CF90
    bge @L_8013CF04
    cmpwi r0, 0x9
    beq @L_8013CF10
    bge @L_8013CF50
    b @L_8013D010
    @L_8013CF04:
    cmpwi r0, 0xd
    beq @L_8013CFD0
    b @L_8013D010
    @L_8013CF10:
    lwz r0, 0x8(r3)
    mr r6, r10
    cmpwi r0, 0x1
    beq @L_8013CF28
    li r3, 0x0
    b @L_8013D094
    @L_8013CF28:
    lwz r0, 0xc(r3)
    cmpwi r0, 0x4
    beq @L_8013CF3C
    li r3, 0x0
    b @L_8013D094
    @L_8013CF3C:
    lhz r0, 0x12(r3)
    cmplwi r0, 0xc
    beq @L_8013D018
    li r3, 0x0
    b @L_8013D094
    @L_8013CF50:
    lwz r0, 0x8(r3)
    mr r7, r10
    cmpwi r0, 0x0
    beq @L_8013CF68
    li r3, 0x0
    b @L_8013D094
    @L_8013CF68:
    lwz r0, 0xc(r3)
    cmpwi r0, 0x4
    beq @L_8013CF7C
    li r3, 0x0
    b @L_8013D094
    @L_8013CF7C:
    lhz r0, 0x12(r3)
    cmplwi r0, 0xc
    beq @L_8013D018
    li r3, 0x0
    b @L_8013D094
    @L_8013CF90:
    lwz r0, 0x8(r3)
    mr r9, r10
    cmpwi r0, 0x1
    beq @L_8013CFA8
    li r3, 0x0
    b @L_8013D094
    @L_8013CFA8:
    lwz r0, 0xc(r3)
    cmpwi r0, 0x5
    beq @L_8013CFBC
    li r3, 0x0
    b @L_8013D094
    @L_8013CFBC:
    lhz r0, 0x12(r3)
    cmplwi r0, 0x4
    beq @L_8013D018
    li r3, 0x0
    b @L_8013D094
    @L_8013CFD0:
    lwz r0, 0x8(r3)
    mr r8, r10
    cmpwi r0, 0x1
    beq @L_8013CFE8
    li r3, 0x0
    b @L_8013D094
    @L_8013CFE8:
    lwz r0, 0xc(r3)
    cmpwi r0, 0x4
    beq @L_8013CFFC
    li r3, 0x0
    b @L_8013D094
    @L_8013CFFC:
    lhz r0, 0x12(r3)
    cmplwi r0, 0x8
    beq @L_8013D018
    li r3, 0x0
    b @L_8013D094
    @L_8013D010:
    li r3, 0x0
    b @L_8013D094
    @L_8013D018:
    addi r3, r3, 0x18
    addi r10, r10, 0x1
    @L_8013D020:
    lwz r0, 0x0(r3)
    cmpwi r0, 0xff
    bne @L_8013CEE8
    cmpwi r6, 0x0
    beq @L_8013D04C
    cmpwi r7, 0x0
    beq @L_8013D04C
    cmpwi r9, 0x0
    beq @L_8013D04C
    cmpwi r8, 0x0
    bne @L_8013D054
    @L_8013D04C:
    li r3, 0x0
    b @L_8013D094
    @L_8013D054:
    mr r3, r31
    b @L_8013D074
    @L_8013D05C:
    lwz r0, 0x20(r3)
    stw r0, 0x4(r5)
    lwz r0, 0x30(r3)
    addi r3, r3, 0x4
    stw r0, 0x14(r5)
    addi r5, r5, 0x18
    @L_8013D074:
    lwz r0, 0x0(r5)
    cmpwi r0, 0xff
    bne @L_8013D05C
    lwz r0, 0x40(r31)
    li r3, 0x1
    stw r0, 0x10(r4)
    lhz r0, 0x44(r31)
    sth r0, 0xe(r4)
    @L_8013D094:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
