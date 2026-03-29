    nofralloc
    cmpwi r6, 0x7
    beq @L_800F9C94
    bge @L_800F9C1C
    cmpwi r6, 0x1
    beq @L_800F9C24
    b @L_800F9C94
@L_800F9C1C:
    cmpwi r6, 0x9
    b @L_800F9C94
@L_800F9C24:
    cmplwi r4, 0x0
    li r7, 0x0
    bne @L_800F9C34
    b @L_800F9C8C
@L_800F9C34:
    lis r6, lbl_80271300@ha
    addi r6, r6, lbl_80271300@l
    b @L_800F9C68
@L_800F9C40:
    cmplwi r3, 0x0
    beq @L_800F9C60
    lbz r0, 0x0(r4)
    addi r4, r4, 0x1
    slwi r0, r0, 1
    lhzx r0, r6, r0
    sth r0, 0x0(r3)
    addi r3, r3, 0x2
@L_800F9C60:
    addi r7, r7, 0x1
    subi r5, r5, 0x1
@L_800F9C68:
    cmpwi r5, 0x0
    beq @L_800F9C7C
    lbz r0, 0x0(r4)
    cmplwi r0, 0xff
    bne @L_800F9C40
@L_800F9C7C:
    cmplwi r3, 0x0
    beq @L_800F9C8C
    li r0, 0x0
    sth r0, 0x0(r3)
@L_800F9C8C:
    mr r3, r7
    blr
@L_800F9C94:
    cmplwi r4, 0x0
    li r7, 0x0
    bne @L_800F9CA4
    b @L_800F9CFC
@L_800F9CA4:
    lis r6, lbl_80271500@ha
    addi r6, r6, lbl_80271500@l
    b @L_800F9CD8
@L_800F9CB0:
    cmplwi r3, 0x0
    beq @L_800F9CD0
    lbz r0, 0x0(r4)
    addi r4, r4, 0x1
    slwi r0, r0, 1
    lhzx r0, r6, r0
    sth r0, 0x0(r3)
    addi r3, r3, 0x2
@L_800F9CD0:
    addi r7, r7, 0x1
    subi r5, r5, 0x1
@L_800F9CD8:
    cmpwi r5, 0x0
    beq @L_800F9CEC
    lbz r0, 0x0(r4)
    cmplwi r0, 0xff
    bne @L_800F9CB0
@L_800F9CEC:
    cmplwi r3, 0x0
    beq @L_800F9CFC
    li r0, 0x0
    sth r0, 0x0(r3)
@L_800F9CFC:
    mr r3, r7
    blr
